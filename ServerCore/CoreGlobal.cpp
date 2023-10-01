#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadlockProfiler.h"
#include "Memory.h"

ThreadManager* GThreadManager = nullptr;
DeadlockProfiler* GDeadLockProfiler = nullptr;
Memory* GMemory = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemory = new Memory();
		GDeadLockProfiler = new DeadlockProfiler();
	
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemory;
		delete GDeadLockProfiler;
	}

}Core;



