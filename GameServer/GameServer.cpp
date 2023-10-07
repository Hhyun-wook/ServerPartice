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
};

enum IO_TYPE
{
	READ,
	WRITE,
	ACCEPT,
	CONNECT,
};


struct OverlappedEx
{
	WSAOVERLAPPED overlapped = {};
	int32 Type = 0; //read,write,accept, connect;
};


void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
{
	cout << "Data Recv Len Callback  = " << recvLen << endl;
	// TODo 에코 서버를 만든다면 WSASend();

	Session* session = (Session*)overlapped; //메모리 주소가 같다는 것을 이용해 이런식으로 사용할 수도 있다.
}



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

void WorkerThreadMain(HANDLE iocpHandle)
{
	while (true)
	{
		DWORD bytesTransferred = 0;
		Session* session = nullptr;
		OverlappedEx* overlappedEx = nullptr;

		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, 
					(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);

		if (ret == FALSE || bytesTransferred == 0)
		{
			// 연결끊김
			continue;
		}

		ASSERT_CRASH(overlappedEx->Type == IO_TYPE::READ);

		cout << "Recv Data IOCP" << bytesTransferred << endl;


		WSABUF wsabuf;
		wsabuf.buf = session->recvBuffer;
		wsabuf.len = BUFSIZE;

		DWORD recvLen = 0;
		DWORD flags = 0;
	

		::WSARecv(session->socket, &wsabuf, 1, &recvLen, &flags,
			&overlappedEx->overlapped, NULL);

	}
}



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

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		return 0;
	}

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		return 0;
	}

	cout <<"Accept" <<endl;

	//Overlapped 모델(Completion Routine 콜백 기반)
	// - 비동기 입출력 함수 완료되면, 쓰레드마다 있는 APC 큐에 일감이 쌓임
	// - Alertable Wait 상태로 들어가서 APC 큐 비우기 (콜백 함수)
	// 단점) APC 큐는 쓰레드마다 있다. Alertable Wait 자체도 조금 부담!
	


	// Overlapped 모델(이벤트 기반)
	// - 비동기 입출력 지원하는 소켓 생성 + 통지 받기 위한 이벤트 객체 생성
	// - 비동기 입출력 함수 호출(1에서 만든 이벤트 객체를 같이 넘겨줌)
	// - 비동기 작업이 바로 완료되지않으면,WSA_IO_PENDING 오류 코드
	// 운영체제는 이벤트 객체를 signale 상태로 만들어서 완료 상태로 알려준다.
	// -WSAWaitForMultipleEvnents 함수를 호출해서 이벤트 객체의 signal 상태를 판별
	// -WSAGetOverlappedResult 를 호출해서 비동기 입출력 결과 확인 및 데이터 처리
	// 단점) 이벤트 방식 소켓 : 이벤트 1: 1대응

	 //Overlapped 모델(Completion Routine 콜백 기반)
	 //- 비동기 입출력 지원하는 소켓 생성
	 //- 비동기 입출력 함수 호출(완료 루틴의 시작주소를 넘겨준다.)
	 //- 비동기 작업이 바로 완료되지않으면,WSA_IO_PENDING 오류 코드
	 //- 비동기 입출력 함수 호출한 쓰레드를 -> Alertable Wait 상태로 만든다.
	 //ex) WaitForSingleObjectEx, WaitForMultipleObjectEx, SleepEx, WSAWaitForMultipleEvents
	 //- 비동기 IO 완료되면 운영체제는 완료 루틴 호출
	 //- 완료 루틴 호출이 모두 끝나면, 쓰레드는 Alertable Wait 상태에서 빠져나온다.
	

	// 1) 오류 발생시 0이 아닌값
	// 2) 전송 바이트 수
	// 3) 비동기 입출력 함수 호출 시 넘겨준 WSAOVERLAPED 구조체 주소의 값
	// 4) 0
	// void CompletionRoutine()


	// 잘안쓰임) WSAAsyncSelect 모델 = 소켓 이벤트를 윈도우 메시지 형태로 처리 (일반 윈도우 메시지랑 같이 처리하니 성능이?? 좀안좋음)
	// Select모델
	// - 장점) 윈도우/ 리눅스 공통
	// - 단점) 성능이 최하 (매번 마다 소켓을 등록하는 비용) 
	// WSAEventSelect 모델 (윈도우 전용)  
	// - 장점) 비교적 뛰어난 성능
	// - 단점) 64개 제한
	// Overlapped (이벤트 기반) (윈도우 전용)
	// - 장점) 성능
	// - 단점) 64개 제한
	// Overlapped (콜백 기반) (윈도우 전용)
	// -장점) 성능
	// -단점) 모든 비동기 소켓 함수에서 사용가능하지 않음(accept), 빈번한 alertable Wait로 인한 성능저하
	// IOCP
	//

	// Reactor Pattern = (뒤늦게, 논블로킹 소켓, 소켓 상태 확인 후 -> 뒤늦게 recv,send 호출
	// Proactor Pattern = (미리, Overlappend WSA- 소켓상태 확인 후 -> 뒤늦게 recv,send 호출

	 IOCP (Completion Port) 모델
	 - APC Queue 대신 -> Completion Port (쓰레드마다 있는건 아니고 1개만 만듦 => 중앙에서 관리하는 APC 큐같은 느낌)
	 - Alertable Wait -> CP(Completion Port) 결과 처리를 GetQueuedCompletionStatus
	 멀티 쓰레드환경에서 사용할 때 궁합이 좋다.

	// CreateIoCompletionPort
	// GetQueueCompletionStatus


	vector<Session*> sessionManager;

	// 1번용도 CP 생성 2번 용도 소켓을 등록
	// 여기서는 CP 생성
	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// WorkerThreads

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]()
		{
			WorkerThreadMain(iocpHandle);

		});
	}

	

	// Main Thread == accept 담당
	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket,(SOCKADDR*)&clientAddr,&addrLen);
		if (clientSocket == INVALID_SOCKET)
			return 0;

		Session* session =new Session();
		session->socket = clientSocket;
		sessionManager.push_back(session);
		
		cout << "Client Connected" << endl;

		// 소켓을 CP에 등록  2번용도
		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle,/*key*/(ULONG_PTR)session, 0);

		WSABUF wsabuf;
		wsabuf.buf = session->recvBuffer;
		wsabuf.len = BUFSIZE;

		// ADD_REF
		DWORD recvLen = 0;
		DWORD flags = 0;

		OverlappedEx* overlappedEX = new OverlappedEx();
		overlappedEX->Type = IO_TYPE::READ;


		::WSARecv(clientSocket, &wsabuf, 1, &recvLen, &flags,
			&overlappedEX->overlapped,NULL);

		// 유저가 게임 접속 종료를 하면 메모리가 해제되는데
		// 주소가 더이상 유효하지 않는다. (오염된 메모리로 될 가능성이 크다.)
		// 그래서 세션을 날리지 못하게 막아줘야한다.-> 래퍼랜스 카운팅으로 관리
		Session* s = sessionManager.back();
		sessionManager.pop_back();
		delete s;

	}
	
	GThreadManager->Join();


	// 윈속 종료
	::WSACleanup();
}