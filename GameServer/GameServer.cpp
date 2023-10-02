#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future> // 리눅스든 윈도우든 상관없음
#include <Windows.h>

#include "ThreadManager.h"
#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"

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

class Knight
{
public:
	int32 _hp = rand() % 100;
};

class Monster
{
public:
	int64 _id = 0;
};

int main()
{
	//Knight* k = ObjectPool<Knight>::Pop();
	//ObjectPool<Knight>::Push(k);
	// shared_ptr<Knight>  sptr = make_shared<Knight>();
	// 이런식으로하면 기본적인 New delete를 이용한다.
	// 그래서 메모리,오브젝트풀링시 이런식으로 만들면안된다.

	Knight* knights[100];

	for (int32 i = 0; i < 100; ++i)
	{
		knights[i] = ObjectPool<Knight>::Pop();
	}


	for (int32 i = 0; i < 100; ++i)
	{
		 ObjectPool<Knight>::Push(knights[i]);
		 knights[i] = nullptr;
	}

	shared_ptr<Knight> sptr = { MakeShared<Knight>() };




	for (int32 i = 0; i < 2; ++i)
	{

		while (true)
		{
			Knight* knight = new Knight();
		
			cout << knight->_hp <<endl;

			this_thread::sleep_for(10ms);

			xdelete(knight);
		}
	}

		
			
	GThreadManager->Join();
}