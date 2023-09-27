#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future> // 리눅스든 윈도우든 상관없음
#include <Windows.h>
#include "ThreadManager.h"
CoreGlobal Core;

// mutex 를 집적 구현해서 사용해야 되는 이유
// 1) 표준 뮤텍스는 재귀적으로 락을 잡을 수 없다.
// 2) 경우에 따라서 가끔 상호배타적이지 않은 특성이 필요한 경우가 있다.
//	  하지만 표준 뮤텍스는 상호배타적이기 때문에 만들필요가있다.
//  상호배타적 : 두사건은 공유된 결과가 없다.

class TestLock
{
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;

		if (_queue.empty())
			return -1;

		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}

	void TestPop()
	{
		WRITE_LOCK;

	
		if (_queue.empty() == false)
			_queue.pop();
	}


private:
	queue<int32> _queue;
};


TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}

int main()
{
	for (int32 i = 0; i < 2; ++i)
	{
		GThreadManager->Launch(ThreadWrite);
	}

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch(ThreadRead);
	}

	GThreadManager->Join();
}