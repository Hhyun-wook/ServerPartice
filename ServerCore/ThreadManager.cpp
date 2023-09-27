#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
	//Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadId = 1;
	LThreadid = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}

void ThreadManager::Launch(function<void(void)>callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
	{
		InitTLS();
		callback();
		DestroyTLS();
	}));
}

void ThreadManager::Join()
{
	for (auto& t : _threads)
	{
		if (t.joinable())
			t.join();
	}

	_threads.clear();
}


