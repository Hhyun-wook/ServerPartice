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

// 참고) CV 는 user-Level Object (커널 오브젝트가 아니다)
// 커널레벨 - 다른 프로젝트에서도 사용가능 
// 유저레벨 - 동일한 프로젝트에서만 사용가능
condition_variable cv; // #include <mutex> 를 추가

// 이벤트랑 비슷하긴 하지만 이벤트는 커널모드 condition_variable 은 유저모드이다.

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고 
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수 통해 다른 쓰레드에게 통지

		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		cv.notify_one();// wait 중인 쓰레드가 있으면 딱 1개를 꺠운다. == //::SetEvent(handle);		// 상태를 시그널상태로 바꿔주는 것이다.

		// lock 이랑 이벤트는 별개의 사항이다.

		//this_thread::sleep_for(10000ms);
	}
}

void Consumer()
{
	while (true)
	{
		//::WaitForSingleObject(handle, INFINITE);		// 시그널상태가 아니면 무한히 대기한다. 대기를 함으로써 의미없는 CPU의 작업을 안할 수 있다.
		//::ResetEvent(handle);
		
		unique_lock<mutex> lock(m); 
		
		// 1) Lock 을 잡고 (위에서 잡았으면 생략)
		// 2) (람다식)조건 확인
		// -> 만족  => 빠져 나와서 이어서 코드를 진행
		// -> 불만족 => Lock을 풀어주고 대기 상태
		cv.wait(lock, []() {return q.empty() == false; });
		// wait 매개변수 인자는 unique_lock 이다.
		// 그런데 notify_one 을 했으면 항상 조건식을 만족하는거 아닐까?
		// 가짜 기상에 빠질 수 있으므로 조건식을 사용해야 한다.
		// notify_one 할 때 lock 을 잡고 있는 것은 아니기 때문에

		//if (q.empty() == false)		// 람다식에서 조건을 사용했기 때문에 조건을 또 안잡아도 된다.
		{
			int32 data = q.front();
			q.pop();
			cout << q.size() << endl;
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


}

