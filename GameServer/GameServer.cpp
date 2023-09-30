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
	// 가상메모리 기본
	// 유저레벨(메모장,  롤, 서버)
	// -------------------------
	// 커널레벨 (OS_Code)

	//SYSTEM_INFO info;
	//::GetSystemInfo(&info);

	//info.dwPageSize;		// 4kb
	//info.dwAllocationGranularity; // 64kb

	/*int* num = new int;
	*num = 100;

	int64 address = reinterpret_cast<int64>(num);
	cout << address;*/



	// new Delete 는 힙영역을 유동적으로 관리한다.
	// (windowAPI를 직접이용해서 하는)VirtualAlloc, VirtualFree 직접 관리한다.
	//  (windowAPI를 직접이용해서 하는) 함수는 그래서 메모리침범이 확인되면 잡아줄 수있다.

	/*Knight* knight = xnew<Knight>(100);

	xdelete(knight);*/
	
	//knight->_hp = 100; //댕글리 포인터를 잡을 수있다.

	// 하지만 오버플로우를 잡을 수없다.
	Knight* knight = (Knight*)xnew<Player>();
	knight->_hp = 100;  // 업캐스팅 했는데 문제 없이동작함 ->오버플로우
	xdelete(knight);
	// [               [여기다가] ] 넣는식으로
	// StumpAllocator를 수정한다.

}