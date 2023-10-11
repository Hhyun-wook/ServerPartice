#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadlockProfiler.h"
#include "Memory.h"
#include "SocketUtils.h"
#include "SendBuffer.h"

ThreadManager* GThreadManager = nullptr;
DeadlockProfiler* GDeadLockProfiler = nullptr;
Memory* GMemory = nullptr;
SendBufferManager* GSendBufferManager = nullptr;
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemory = new Memory();
		GDeadLockProfiler = new DeadlockProfiler();
		GSendBufferManager = new SendBufferManager();
		SocketUtils::Init();
	
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemory;
		delete GDeadLockProfiler;
		delete GSendBufferManager;
		SocketUtils::Clear();
	}

}GCoreGlobal;



