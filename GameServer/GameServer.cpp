#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음

#include <windows.h>

mutex m;
queue<int32> q;
HANDLE handle; 

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		::SetEvent(handle);		// 상태를 시그널상태로 바꿔주는 것이다.

		this_thread::sleep_for(10000ms);
	}
}

void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(handle, INFINITE);		// 시그널상태가 아니면 무한히 대기한다. 대기를 함으로써 의미없는 CPU의 작업을 안할 수 있다.
		//::ResetEvent(handle);
		unique_lock<mutex> lock(m); 

		if (q.empty() == false)		// queue 가 비어 있는데도 매번 체크하는 작업은 CPU에 부하가 걸릴 수 있다. 
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// 이벤트는 커널 오브젝트이다.
	// Usage Count
	// Signal / Non-Signal  2가지는 커널오브젝트가 가지고 있는 것이다.
	// Auto  / Manual  << bool 
	// 이벤트는 다른 커널오브젝트에 비해 가볍다.
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE /* ManualReset  FALSE == auto True == 수동*/, FALSE /*초기 상태*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);

}

