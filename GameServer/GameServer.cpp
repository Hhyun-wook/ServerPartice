#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future> // 리눅스든 윈도우든 상관없음
#include <Windows.h>

thread_local int32 LThreadid = 0; // TLS 변수

thread_local queue<int32> q; // 이런식으로 응용가능

void ThreadMain(int32 threadid)
{
	LThreadid = threadid;

	while (true)
	{
		cout << "ID " << LThreadid << endl;
		this_thread::sleep_for(10s);
	}
}


int main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; ++i)
	{
		int32 threadid = i + 1;
		threads.push_back(thread(ThreadMain,threadid));
	}

	for(auto& t : threads)
	{
		t.join();
	}

}