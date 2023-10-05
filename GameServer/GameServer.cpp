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
int main()
{
	// 윈속 초기화 (ws2_32 라이브러리 초기화)
	// 관련 정보가 wsaData에 채워짐
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}


	SOCKADDR_IN serverAddr; // IPv4 버젼
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	//::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 현대식 버전
	serverAddr.sin_port = ::htons(7777);  // 80 : HTTP 특정번호는 규약이 있다.

	if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		HandleError("Bind");
		return 0;
	}

	while (true)
	{
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);
		
		this_thread::sleep_for(1s);
		
		char recvBuffer[1000];

		int32 recvLen =  ::recvfrom(serverSocket, recvBuffer, sizeof(recvBuffer), 0
		,(SOCKADDR*)&clientAddr,&addrLen);

		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}

		cout << " Recv Data :  Data : " << recvBuffer << endl;
		cout << " Recv Data :  Len : " << recvLen << endl; 

		int32 errorCode = ::sendto(serverSocket, recvBuffer, recvLen, 0,
			(SOCKADDR*)&clientAddr, sizeof(clientAddr));

		if (errorCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return 0;
		}

		cout << "Send Data Len : " << sizeof(recvLen) << endl;

	}


	// 소켓 리소스 반환
	::closesocket(serverSocket);

	// 윈속 종료
	::WSACleanup();
}