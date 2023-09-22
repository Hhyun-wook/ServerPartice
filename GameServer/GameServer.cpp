#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음

#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for (int32 i = 0; i < 100; ++i)
	{
		UserManager::instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 100; ++i)
	{
		AccountManager::instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << " Done" << endl;

	mutex m1;
	mutex m2;

	std::lock(m1,m2); // m1.lock(), m2.lock(); 작성된 순서상관없이 내부적으로 잠궈준다

	// adopt_lock : 이미 lock 상태니까, 나중에 소멸될 때 풀어주기만 해
	lock_guard<mutex> g1(m1, std::adopt_lock);
	lock_guard<mutex> g1(m2, std::adopt_lock);

}

