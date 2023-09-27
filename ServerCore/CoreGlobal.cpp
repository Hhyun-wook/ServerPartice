#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadlockProfiler.h"


ThreadManager* GThreadManager = nullptr;
DeadlockProfiler* GDeadLockProfiler = nullptr;


class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GDeadLockProfiler = new DeadlockProfiler();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GDeadLockProfiler;
	}

}Core;



