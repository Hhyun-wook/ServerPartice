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
#include "ThreadManager.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"

int main()
{
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1",7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->DisPatch();
				}

			});
	}


	char SendData[1000] = "Hello World";

	while (true)
	{
		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
		
		BYTE* buffer = sendBuffer->GetBuffer();
		
		// 여기서 사이즈를 잘못 기입하면? 엉뚱한 데이터를 파싱할 수 있다. 보안 관련부분에서 주의해야함
		((PacketHeader*)buffer)->size = (sizeof(SendData) + sizeof(PacketHeader));
		
		((PacketHeader*)buffer)->id = 1;
		::memcpy(&buffer[4], SendData, sizeof(SendData));
		sendBuffer->Close((sizeof(SendData) + sizeof(PacketHeader)));

		//for(int i=0; i<5; ++i) // 패킷을 강제로 뭉쳐서 하는경우
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}
	

	GThreadManager->Join();
}