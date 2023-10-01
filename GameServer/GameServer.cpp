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

class Player
{
public:
	Player() {}
	~Player() {}
};

class Knight :public Player
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) :_hp(hp)
	{
		cout << "Knight()" << endl;
	}


	~Knight()
	{
		cout << "~Knight()" << endl;
	}


public:
	int32 _hp = 100;
	int32 _mp = 10;
};


int main()
{
	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					vector<Knight> v(10);

					xMap<int32, Knight> m;
					m[100] = Knight();

					this_thread::sleep_for(10ms);
				}
			});
	}

	GThreadManager->Join();

	vector<Knight,StlAllocator<Knight>> v(100);

	//map<int32, int32> m;

}