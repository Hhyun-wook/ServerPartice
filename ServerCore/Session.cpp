#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

Session::Session() :
	_recvBuffer(BUFFER_SIZE)
{
	_socket = SocketUtils::createSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

//void Session::Send(BYTE* buffer, int32 len)
//{
//	// 고려할 사항
//	// 1) 버퍼관리
//	// 2) sendEvent 관리 : 단일? , 여러개 ? ,WSASend 중첩?
//	// 복사비용 관리해야한다.
//
//	// Test
//	SendEvent* sendEvent = xnew<SendEvent>();
//	sendEvent->owner = shared_from_this(); // ADD_REF
//	sendEvent->buffer.resize(len);	
//	memcpy(sendEvent->buffer.data(), buffer, len);
//	
//
//	WRITE_LOCK;		// 멀티쓰레드 환경에서 안전하지않다.
//	RegisterSend();
//}

void Session::Send(SendBufferRef sendBuffer)
{
	// 현재 RegisterSend 가 걸리지 않은 상태라면 걸어준다.
	WRITE_LOCK;
	
	_sendQueue.push(sendBuffer);

	//CAS 사용 Atomic 이라서
	//if (_sendRegistered == false)
	//{
	//	//	_sendRegistered = true; 
	//	RegisterSend();
	//}

	if (_sendRegistered.exchange(true)== false)
		RegisterSend();
	

}

bool Session::Connect()
{
	return RegisterConnect();

}

void Session::Disconnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
		return;


	//Temp
	wcout << "DisConnect : " << cause << endl;

	OnDisconnected(); //컨텐츠 코드에서 오버라이딩
	
	GetService()->ReleaseSession(GetSessionRef());

	RegisterDisConnect();
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* IocpEvent, int32 numofBytes)
{
	switch (IocpEvent->_eventtype)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::DisConnect:
		ProcessDisConnect();
		break;
	case EventType::Recv:
		ProcessRecv(numofBytes);
		break;
	case EventType::Send:
		ProcessSend(numofBytes);
		break;
	default:
		break;
	}
	
}

bool Session::RegisterConnect() 
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	if (SocketUtils::SetReuseAddress(_socket, true) == false)//주소재활용
		return false;
	
	if (SocketUtils::BindAnyAddress(_socket, 0/* 남는거 줘 */) == false) //어떤 주소 어떤 포트도 상관없다
		return false;

	_connectEvent.Init();
	_connectEvent.owner = shared_from_this(); //Add_ref

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	if (false == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), 
		sizeof(sockAddr),nullptr, 0, &numOfBytes, &_connectEvent))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_connectEvent.owner = nullptr; // Relase Ref
			return false;
		}
	}

	return true;
}

bool Session::RegisterDisConnect()
{
	_disConnectEvent.Init();
	_disConnectEvent.owner = shared_from_this(); // Add_ref

	if (false == SocketUtils::DisconnectEx(_socket, &_disConnectEvent, TF_REUSE_SOCKET, 0))
	{
		const int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			HandleError(errCode);
			return false;
		}
	
	}

	return true;
}

void Session::RegisterRecv() // 실질적으로 한번에 한쓰레드만 들어오기 때문에 멀티쓰레드를 신경 X
{
	if (IsConnected() == false)
		return;

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos()); 
	wsaBuf.len = _recvBuffer.FreeSize(); 

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	if (::WSARecv(_socket, &wsaBuf, 1, OUT &numOfBytes, OUT &flags, &_recvEvent, nullptr) == SOCKET_ERROR)
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_recvEvent.owner = nullptr; // RELEASE_REF
		}
	}

}

void Session::RegisterSend()
{
	if (IsConnected() == false)
		return;

	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();

	// 보낼 데이터를 sendEvent에 등록
	{
		WRITE_LOCK;		//나중에 바뀔 코드 때문에 2중락을 걸어줌

		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO: 예외처리 만들기
			

			_sendQueue.pop();
			_sendEvent.SendBuffers.push_back(sendBuffer);
		}

	}


	// Scatter-Gather (흩어져 있는 데이터들을 모아서 한방에 보낸다.)
	xVector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent.SendBuffers.size());
	for (SendBufferRef sendBuffer : _sendEvent.SendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->GetBuffer());
		wsaBuf.len = static_cast<ULONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}



	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT &numOfBytes, 0, &_sendEvent, nullptr))
	{
		int32 errocode = WSAGetLastError();
		if (errocode != WSA_IO_PENDING)
		{
			HandleError(errocode);
			_sendEvent.owner = nullptr; // RELEASE_REF
			_sendEvent.SendBuffers.clear(); //RELEASE_REF
			_sendRegistered.store(false);
		}
	}
}

void Session::ProcessConnect()
{
	_connectEvent.owner = nullptr; //Release Ref
	
	_connected.store(true);

	//세션등록
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 오버라이딩
	OnConnected();

	// 수신 등록
	RegisterRecv();

}

void Session::ProcessDisConnect()
{
	_disConnectEvent.owner = nullptr; //Relase_ref

}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr; // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv Len is Zero");
		return; 
	}

	if (_recvBuffer.OnWrite(numOfBytes))
	{
		Disconnect(L"OnWrite overFlow");
		return;
	}
	
	int32 dataSize = _recvBuffer.DataSize();



	int32 ProcessLen = OnRecv(_recvBuffer.ReadPos(), dataSize); 	// 컨텐츠 코드에서 오버라이딩
	if (ProcessLen < 0 || dataSize < ProcessLen || _recvBuffer.OnRead(ProcessLen) == false)
	{
		Disconnect(L"OnRead OverFlow");
		return;
	}

	//커서 정리
	_recvBuffer.Clean();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessSend( int32 numOfBytes)
{
	_sendEvent.owner = nullptr; // RELEASE_REF
	_sendEvent.SendBuffers.clear(); //RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Send is zero");
		return;
	}

	// 컨텐츠 코드에서 오버라이딩
	OnSend(numOfBytes);

	WRITE_LOCK;

	if (_sendQueue.empty())
	{
		_sendRegistered.store(false);
	}
	else
	{
		RegisterSend();
	}
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
		Disconnect(L"HandleError");
		break;
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TO_DO : Log
		cout << "Handle Error " << errorCode << endl;
		break;

	}

}
