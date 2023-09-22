#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음


class SpinLock
{
public:
	

	void lock()
	{
		// CAS (Compare -And -Swap)

		bool expected = false;
		bool desired = true;

		//// CAS 의사코드

		//if (_locked == expected)
		//{
		//	_locked = desired;
		//	return true;
		//}
		//else
		//{
		//	expected = _locked;
		//	return false;
		//}

		// 앞에 있는 쓰레드가 빨리끝나면 컨텍스트 스위칭이 일어나지 않아 CPU의 성능이 증가하나
		// 앞에 있는 쓰레드가 늦게 끝나면 계속해서 체크를 해야하기 때문에 CPU의 성능이 줄어들음
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}

	
	}

	void unlock()
	{
		//_locked = false;
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false; //  volatile : c++ 최적화를 하지마라! 거의 사용하지않는다.
							
};

mutex m;
int32 sum = 0;
SpinLock spinLock;


void Add()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		++sum;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		--sum;
	}
}


int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}

