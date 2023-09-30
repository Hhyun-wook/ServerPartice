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

class Knight
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

	//// 이런식으로 관리해 로그를 찍어볼 수 있다.
	//// 하지만 매 클래스마다 넣는것은 불편하다.
	//static void* operator new(size_t size)
	// {
	//	 cout << "new~!" << size << endl;
	//	 void* ptr = ::malloc(size);
	//	 return ptr;
	// }

	//static void operator delete(void* ptr)
	// {
	//	 cout << "delete!" << endl;
	//	 ::free(ptr);
	// }



public:
	int32 _hp = 100;
	int32 _mp = 10;
};

// 전역적인 오퍼레이터는 위험하다.
//// new operator overloading (Global)
//void* operator new(size_t size)
//{
//	cout << "new~!" << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete(void* ptr)
//{
//	cout << "delete!" << endl;
//	::free(ptr);
//}
//
//void* operator new[](size_t size)
//{
//	cout << "new[]!" << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete[](void* ptr)
//{
//	cout << "delete !" << endl;
//	::free(ptr);
//}


int main()
{
	// new :	opeator new 를 사용해 호출 -> 생성자 호출
	Knight* knight = xnew<Knight>();

	xdelete(knight);
 
		


}