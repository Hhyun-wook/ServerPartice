//#include "pch.h"
//#include <iostream>
//#include "Corepch.h"
//#include <thread> // 리눅스든 윈도우든 상관없음
//#include <atomic> // 리눅스든 윈도우든 상관없음
//#include <mutex>  // 리눅스든 윈도우든 상관없음
//#include <future> // 리눅스든 윈도우든 상관없음
//#include <Windows.h>
//#include "ThreadManager.h"
//
//
//#include <winsock2.h>
//#include <mswsock.h>
//#include <ws2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")


//
//DECLSPEC_ALIGN(16)
//class Data  // 1.상속을 받거나 : public SLIST_ENTRY
//{
//public:
//	SLIST_ENTRY _entry;		// 2.멤버변수로 받거나
//	int64 _rand = rand() % 1000;
//};
//
//SLIST_HEADER* GHeader;
//
//int main()
//{
//
//	GHeader = new SLIST_HEADER();
//	ASSERT_CRASH(((uint64)GHeader % 16) == 0);
//	InitializeSListHead(GHeader);
//		
//	for (int32 i = 0; i < 3; ++i)
//	{
//		GThreadManager->Launch([]()
//			{
//				while (true)
//				{
//					Data* data = new Data();
//					ASSERT_CRASH(((uint64)data % 16) == 0);
//
//					InterlockedPushEntrySList(GHeader, (PSLIST_ENTRY)data);
//					this_thread::sleep_for(10ms);
//				}
//
//			});
//	}
//
//	for (int32 i = 0; i < 2; ++i)
//	{
//		GThreadManager->Launch([]()
//			{
//				while (true)
//				{
//					Data* pop = nullptr;
//					pop = (Data*)InterlockedPopEntrySList(GHeader);
//
//					if (pop)
//					{
//						cout << pop->_rand << endl;
//						delete pop;
//					}
//					else
//					{
//						cout << "NONE" << endl;
//					}
//				}
//
//			});
//	}
//
//	
//	GThreadManager->Join();
//
//}

//enum PLAYER_TYPE
//{
//	KNIGHT,
//	MAGE,
//
//};
//
//using TL = TypeList<class Player, class Mage, class Knight, class Archer>;
//
//class Player
//{
//
//
//public:
//	Player()
//	{
//		INIT_TL(Player);
//	}
//
//	virtual ~Player(){}
//	DECLEAR_TL
//
//
//};
//
//class Knight :public Player
//{
//public:
//	Knight() { INIT_TL(Knight); }
//};
//
//
//class Mage :public Player
//{
//public:
//	Mage() { INIT_TL(Mage); }
//};
//
//
//class Archer :public Player
//{
//public:
//	Archer() { INIT_TL(Archer); }
//};
//
//
//class Monster
//{
//public:
//	int64 _id = 0;
//};
//
//int main()
//{
//	//Knight* k = ObjectPool<Knight>::Pop();
//	//ObjectPool<Knight>::Push(k);
//	// shared_ptr<Knight>  sptr = make_shared<Knight>();
//	// 이런식으로하면 기본적인 New delete를 이용한다.
//	// 그래서 메모리,오브젝트풀링시 이런식으로 만들면안된다.
//
//	//Knight* knights[100];
//
//	//for (int32 i = 0; i < 100; ++i)
//	//{
//	//	knights[i] = ObjectPool<Knight>::Pop();
//	//}
//
//
//	//for (int32 i = 0; i < 100; ++i)
//	//{
//	//	 ObjectPool<Knight>::Push(knights[i]);
//	//	 knights[i] = nullptr;
//	//}
//
//	//shared_ptr<Knight> sptr = { MakeShared<Knight>() };
//
//
//	//{
//	//	Player* player = new Player();
//
//	//	bool canCast = CanCast<Knight*>(player);
//	//	Knight* knight = TypeCast<Knight*>(player);
//
//	//	
//
//	//	delete player;
//	//}
//
//	//{
//	//	shared_ptr<Knight> knight = MakeShared<Knight>();
//
//	//	shared_ptr<Player> player = TypeCast<Knight>(knight);
//	//	bool canCast = CanCast<Player>(knight);
//	//}
//
//
//
//
//}

#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << "Socket ErrorCode : " << errCode << endl;
}

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	int32 sendBytes = 0;
};



// TCP
//int main()
//{
//	// 윈속 초기화 (ws2_32 라이브러리 초기화)
//	// 관련 정보가 wsaData에 채워짐
//	WSAData wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
//	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
//	// protocol : 0
//	// return : descriptor
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//	{
//		int32 errCode = ::WSAGetLastError();
//		cout << "Socket ErrorCode : " << errCode << endl;
//		return 0;
//	}
//
//	// 나의 주소는? (IP주소 + Port)->XX 아파트 YY 호
//	SOCKADDR_IN serverAddr; // IPv4
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); //< 니가 알아서 해줘
//	serverAddr.sin_port = ::htons(7777); // 80 : HTTP
//
//	// 안내원 폰 개통! 식당의 대표 번호
//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		int32 errCode = ::WSAGetLastError();
//		cout << "Bind ErrorCode : " << errCode << endl;
//		return 0;
//	}
//
//	// 영업 시작!
//	if (::listen(listenSocket, 10) == SOCKET_ERROR)
//	{
//		int32 errCode = ::WSAGetLastError();
//		cout << "Listen ErrorCode : " << errCode << endl;
//		return 0;
//	}
//
//	// -----------------------------
//
//	while (true)
//	{
//		SOCKADDR_IN clientAddr; // IPv4
//		::memset(&clientAddr, 0, sizeof(clientAddr));
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//		if (clientSocket == INVALID_SOCKET)
//		{
//			int32 errCode = ::WSAGetLastError();
//			cout << "Accept ErrorCode : " << errCode << endl;
//			return 0;
//		}
//
//		// 손님 입장!
//		char ipAddress[16];
//		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
//		cout << "Client Connected! IP = " << ipAddress << endl;
//
//
//		// TODO
//		while (true)
//		{
//			char recvBuffer[1000];
//
//			this_thread::sleep_for(1s);
//
//			// 단순히 데이터를 받는 함수 그래서 listenSocket이 아닌 클라이언트소켓이다.
//			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
//
//			if (recvLen <= 0)
//			{
//				int32 errCode = ::WSAGetLastError();
//				cout << "Accept ErrorCode : " << errCode << endl;
//				return 0;
//			}
//
//			cout << "Recv Data :  Data : " << recvBuffer << endl;
//			cout << "Recv Data :  Len : " << recvLen << endl;
//
//
//			/*int32 reslutCode = ::send(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
//			if (reslutCode == SOCKET_ERROR)
//			{
//				int32 errCode = ::WSAGetLastError();
//				cout << "Sokcet ErrCode : " << errCode << endl;
//				return 0;
//			}*/
//
//		}
//
//	}
//
//	// -----------------------------
//
//
//	// 윈속 종료
//	::WSACleanup();
//}



// UDP
//int main()
//{
//	// 윈속 초기화 (ws2_32 라이브러리 초기화)
//	// 관련 정보가 wsaData에 채워짐
//	WSAData wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
//	if (serverSocket == INVALID_SOCKET)
//	{
//		HandleError("Socket");
//		return 0;
//	}
//
//
//	SOCKADDR_IN serverAddr; // IPv4 버젼
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//	//::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 현대식 버전
//	serverAddr.sin_port = ::htons(7777);  // 80 : HTTP 특정번호는 규약이 있다.
//
//	if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		HandleError("Bind");
//		return 0;
//	}
//
//	while (true)
//	{
//		SOCKADDR_IN clientAddr;
//		::memset(&clientAddr, 0, sizeof(clientAddr));
//		int32 addrLen = sizeof(clientAddr);
//		
//		this_thread::sleep_for(1s);
//		
//		char recvBuffer[1000];
//
//		int32 recvLen =  ::recvfrom(serverSocket, recvBuffer, sizeof(recvBuffer), 0
//		,(SOCKADDR*)&clientAddr,&addrLen);
//
//		if (recvLen <= 0)
//		{
//			HandleError("RecvFrom");
//			return 0;
//		}
//
//		cout << " Recv Data :  Data : " << recvBuffer << endl;
//		cout << " Recv Data :  Len : " << recvLen << endl; 
//
//		int32 errorCode = ::sendto(serverSocket, recvBuffer, recvLen, 0,
//			(SOCKADDR*)&clientAddr, sizeof(clientAddr));
//
//		if (errorCode == SOCKET_ERROR)
//		{
//			HandleError("SendTo");
//			return 0;
//		}
//
//		cout << "Send Data Len : " << sizeof(recvLen) << endl;
//
//	}
//
//
//	// 소켓 리소스 반환
//	::closesocket(serverSocket);
//
//	// 윈속 종료
//	::WSACleanup();
//}





int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// 블로킹 소켓
	// accept -> 접속한 클라가 있을 때
	// connect -> 서버 접속 성공했을 때
	// send, sendto -> 요청한 데이터를 송신 버퍼에 복사했을 때
	// recv, recvfrom -> 수신 버퍼에 도착한 데이터가 있고, 이를 유저레벨 버퍼에 복사했을 때
	// 완료될 때 까지 대기를 한다? -> 게임에서 불리하다.


	// 논블로킹 소켓
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
	{
		return 0;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		return 0;
	}

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		return 0;
	}

	cout <<"Accept" <<endl;

	/* WSAEventSelect = (WSAEventSelect 함수가 핵심이 되는)
	 소켓과 관련된 네트워크 이벤트를 [이벤트 객체] 를 통해 감지

	 이벤트 객체 관련 함수들
	 생성 : WSACreateEvent (수동 리셋 Manual-Reset + Non-Signaled 상태 시작)
	 삭제 : WSACloseEvent
	 신호 상태 감지 : WSAWaitForMultipleEvents
	 구체적인 네트워크 이벤트 알아내기 : WSAEnumNetworkEvents

	 소켓 <-> 이벤트 객체 연동
	 소켓개수만큼 이벤트객체를 만들어야한다.
	
	 WSAEventSelect(socket, event, networkEvents);
	 - 관심있는 네트워크 이벤트
	 FD_ACCEPT : 접속한 클라가 있음 accept
	 FD_READ   : 데이터 수신 가능 recv , recvfrom
	 FD_WRITE : 데이터 송신 가능 send,sendto
	 FD_CLOSE : 상대가 접송 종료
	 FD_CONNECT : 통신을 위한 연결 절차 완료
	 FD_OOB

	  주의 사항
	 WSAEventSelect 함수를 호출하면, 해당 소켓은 자동으로 넌블로킹 모드 전환
	 accept() 함수가 리턴하는 소켓은 listenSocket과 동일한 속성을 갖는다.
	 따라서 clientSocket은 FD_READ, FD_WRITE 등을 다시 등록 필요
	 드물게 WSAEWOULDBLOCK 오류가 뜰 수 있으니 예외처리필요
	 중요)
	 - 이벤트 발생 시 , 적절한 소켓 함수를 호출해야한다.
	 - 그렇지 않으면 다음 번에는 동일 네트워크에서 이벤트가 발생하지 않는다.
	 ex) FD_READ 이벤트가 떳으면 recv()호출해야하고, 안하면 FD_READ 두번 다시 발생하지않는다.

	 1) count, event
	 2) waitAll : 모두 기다림? 하나만 완료되어도 OK?
	 3) timeout 타임아웃
	 4) 지금은 false;
	 return 완료된 첫번째 인덱스
	 WSAWaitForMultipleEnvets()


	 1) socket
	 2) eventObject : socket 과 연동된 이벤트 객체 핸들을 넘겨주면 , 이벤트 객체를 non-signaled
	 3) networkEvent : 네트워크 이벤트 / 오류 정보가 저장
	 WSAEnumNetworkEvents()*/

	vector<WSAEVENT> wsaEvents;
	vector<Session> sessions;
	sessions.reserve(100);
	
	WSAEVENT listenEvent = ::WSACreateEvent();
	wsaEvents.push_back(listenEvent);
	sessions.push_back(Session{ listenSocket });

	if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
		return 0;


	while (true)
	{
		
		int32 index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, false);
		
		if (index == WSA_WAIT_FAILED)
			continue;

		index -= WSA_WAIT_EVENT_0;

		// ::WSAResetEvent(wsaEvent[index]) 리셋해주는기능이 WSAEnumNetworkEvents자동적으로 있음
		WSANETWORKEVENTS networkEvents;
		if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
			continue;

		// Listener 소켓 체크
		if (networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			// Error_Check
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;

			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);

			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << " client Connected" << endl;
				WSAEVENT listenEvent = ::WSACreateEvent();
				wsaEvents.push_back(listenEvent);
				sessions.push_back(Session{ listenSocket });

				if (::WSAEventSelect(listenSocket, listenEvent, FD_READ |FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
					return 0;

			}
		}

		// Client Session 소켓 체크
		if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
		{
			if ((networkEvents.lNetworkEvents & FD_READ) && (networkEvents.iErrorCode[FD_READ_BIT] != 0))
				continue;

			if ((networkEvents.lNetworkEvents & FD_WRITE) && (networkEvents.iErrorCode[FD_WRITE_BIT] != 0))
				continue;

			Session& s = sessions[index];

			//Read
			if (s.recvBytes == 0)
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// Todo : 세션 제거
					continue;
				}

				s.recvBytes = recvLen;
				cout << "Recv Data = " << recvLen << endl;
			}

			// write
			if (s.recvBytes > s.sendBytes)
			{
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR && :: WSAGetLastError() != WSAEWOULDBLOCK)
				{
					continue;
				}

				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}

				cout << "Send Data " << sendLen << endl;

			}

		}

	}


	::closesocket(listenSocket);

	// 윈속 종료
	::WSACleanup();
}