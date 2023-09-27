#pragma once
#include "Types.h"

/* 
	Reader_Writer SpinLock
*/

/* 
	[wwwwwwww][wwwwwwww][rrrrrrrr][rrrrrrrr]
	w: WriteFlag (Exclusive Lock Owner ThreadID)
	r : ReadFlag (Shared Lock Count)
*/

// 설계 정책
// W->W 가능
// W->R Write 에서 Read로 가는 가능하지만
// R->W Read 에서 Write로 가는 것은 불가능하다.
class Lock
{
	enum :uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock(const char* name);
	void WriteUnLock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/*
	LockGuards
*/
//RAII 패턴
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) :_lock(lock), _name(name)
	{
		_lock.ReadLock(_name);
	}
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) :_lock(lock) ,_name(name)
	{
		_lock.WriteLock(name);
	}
	~WriteLockGuard() { _lock.WriteUnLock(_name); }

private:
	Lock& _lock;
	const char* _name;
};