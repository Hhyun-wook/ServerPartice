﻿#include "pch.h"
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

#include "LockFreeStack.h"


DECLSPEC_ALIGN(16)
class Data
{
public:
	SListEntry _entry;
	int64 _rand = rand() % 1000;
};

SListHeader* GHeader;

int main()
{

	GHeader = new SListHeader();
	ASSERT_CRASH(((uint64)GHeader % 16) == 0);
	InitializeHead(GHeader);
		
	for (int32 i = 0; i < 3; ++i)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* data = new Data();
					ASSERT_CRASH(((uint64)data % 16) == 0);

					PushEntrySList(GHeader, (SListEntry*)data);
					this_thread::sleep_for(10ms);
				}

			});
	}

	for (int32 i = 0; i < 2; ++i)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* pop = nullptr;
					pop = (Data*)PopEntrySList(GHeader);

					if (pop)
					{
						cout << pop->_rand << endl;
						delete pop;
					}
					else
					{
						cout << "NONE" << endl;
					}
				}

			});
	}

	
	GThreadManager->Join();

}