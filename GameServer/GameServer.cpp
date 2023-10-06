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

	// Select 모델 = select 함수가 핵심이 되는
	// 소켓 함수 호출이 성공할 시점을 미리 알 수 있다.
	// 문제 상황)
	// 수신버퍼에 데이터가 없는데, read 한다거나!
	// 송신버퍼가 꽉 찼는데, write 한다거나!
	// - 블로킹 소켓 : 조건이 만족되지 않아서 블로킹되는 상황 예방
	// - 논블로킹 소켓 : 조건이 만족되지 않아서 불필요하게 반복 체크하는 상황을 예방

	// socket set
	// 1) 읽기[1 2 3] 쓰기[] 예외[] 관찰 대상 등록
	// OutofBand는 send() 마지막인자를 경우에 따라서 MSG_OOB로 보내는 특별한 데이터이다.
	// 받는 쪽에서도 recv OOB 세팅을 해야 읽을 수 있다.
	// 2) select(readSet, writeSet, exceptSet); -> 관찰 시작
	// 3) 즉 적어도 하나의 소켓이 준비되면 리턴 -> 낙오자는 알아서 제거됨
	// 4) 남은 소켓 체크해서 진행

	// fd_set read :
	// FD_ZERO : 비운다.
	// ex) FD_ZERO(set)
	// FD_TEST 소켓 s를 넣는다.
	// ex) FD_SET(s,&set);
	// FD_CLR : 소켓 s를 제거
	// ex)  FD_CLR(s,&set);
	// FD_ISSET :소켓 s 가 set에 들어있으면 0이 아닌 값을 리턴한다.
	
	vector<Session> sessions;
	sessions.reserve(100);

	// 장점: 무작정 accept , recv ,send를 호출하는것이아니라 
	// 호출된 준비가 되어있다는것을 확인하고 안전하게 recv, send를 한다는 것이다.
	// 블로킹,논블로킹 상관없이 사용할 수 있다. 2) 간단하다.
	// 단점 : fd_set에 있는 배열이 숫자가 작아 동접자 수가 fd_set을 많이 선언해야한다.


	fd_set reads;
	fd_set writes;

	while (true)
	{

		// 매번마다 다시밀고 다시 등록한다.
		// select는 낙오자를 제거하기 때문이다.
		// 소켓 셋 초기화
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		//ListenSocket 등록
		FD_SET(listenSocket, &reads);

		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		// [옵션] : 마지막 timeout 인자 설정 가능
		/*timeval timeOut;
		timeOut.tv_sec;
		timeOut.tv_usec*/
		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
			break;

		// Listener 소켓 체크
		if (FD_ISSET(listenSocket, &reads))
		{
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

			if (clientSocket != INVALID_SOCKET)
			{
				cout << "client Connect " << endl;
				sessions.push_back(Session{clientSocket});
			}
		}

		// 나머지 소켓 체크
		for (Session& s : sessions)
		{
			// Read 체크
			if (FD_ISSET(s.socket, &reads))
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					//Todo :sessions 에서 제거
					continue;
				}

				s.recvBytes = recvLen;
			}


			// Write 체크
			if (FD_ISSET(s.socket, &writes))
			{
				// 블로킹 모드 -> 모든 데이터를 다 보낸다.
				// 논블로킹 모드 -> 상대방 수신 버퍼상황에 따라서 일부만 보낼 수 있다.
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);

				if (sendLen == SOCKET_ERROR)
				{
					// Todo  Session 제거
					continue;
				}

				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}

			}
		}
	}


	::closesocket(listenSocket);

	// 윈속 종료
	::WSACleanup();
}