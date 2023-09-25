#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future> // 리눅스든 윈도우든 상관없음
#include <Windows.h>

 //1) 가시성 , 2) 코드 재배치 란 문제가 있다.
 //1로 대입하더라도 1로 나오지 않을 수 있다.-> 가시성
 //코드 재배치 -> 컴파일러가 최적화할 때 재배치할 수 있다.
	//		  -> 또는 CPU가 재배치할 수 있다.
	//		  -> 싱글쓰레드에서는 문제가 없을 수 있으나 멀티쓰레드에서는 다른 쓰레드에서 건들 수 있기 때문에 이상해진다.
	//		

int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

volatile bool ready =false;

void Thread_1()
{
	while (!ready)
		;

	y = 1;  //store y		
	r1 = x; // Load X
}

void Thread_2()
{
	while (!ready)
		;

	x = 1;  //store x
	r2 = y; // Load y
}



int main()
{
	int32 count = 0;

	while (true)
	{
		ready = false;
		count++;

		x = y = r1 = r2 = 0;

		thread t1(Thread_1);
		thread t2(Thread_2);

		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
			break;

	}


	cout << count << " 만에 빠져나옴" << endl;

}

