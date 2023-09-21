#include "pch.h"
#include <iostream>
#include "Corepch.h"

#include <thread> // 리눅스든 윈도우든 상관없음

//#include <Windows.h>

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread_2(int32 num)
{
	cout << num << endl;
}

int main()
{
	
	// System Call (OS 커널 요청)
	// ::CreateThread()  : 윈도우에 종속적인 함수

	// vector<std::thread> 사용예제
	vector<std::thread> v;
	
	for (int32 i = 0; i < 10; i++)
	{
		v.push_back(std::thread(HelloThread_2, i));
	}

	for (int32 i = 0; i < 10; i++)
	{
		if (v[i].joinable())
			v[i].join();
	}


	//std::thread t1;
	//auto count1 = t1.get_id();
	//t1 = std::thread(HelloThread_2,10);

	//std::thread t(HelloThread); //  ==  std::thread t;  t= std::thread(Helloworld);
	//int32 count = t.hardware_concurrency();	// CPU 코어 개수?  -> 100% 장담은 못하지만 왠만한 환경에서 나온다.
	//auto id = t.get_id();								// 각 쓰레드마다 id  (들쑥날쑥한 값이나(*연속적이지 않음) 쓰레드마다 값이 다르다.) 
	//
	////t.detach();			// 쓰레드의 연결고리를 끊어주겠다 -> std::thread 객체에서 실제 쓰레드를 분리한다.
	////					// 분리를 하면 만들어준 쓰레드의 정보 추출이 불가능하다. 
	////t.joinable();		// 쓰레드객체가 존재하는지 아닌지 확인하는 함수
	//
	//if (t.joinable())
	//{
	//	t.join();
	//}

	cout << "Hello Main" << endl;
}

