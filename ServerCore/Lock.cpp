#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{	
	
	/*if (_lockFlag == EMPTY_FLAG)
	{
		const uint32 desired = ((LThreadid << 16) & WRITE_THREAD_MAX);
		_lockFlag = desired;
	}*/
	// 동일할 쓰레드가 소유하고 있다면 무조건 성공하게 만든다.
	const uint32 lockThreadid = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

	if (LThreadid == lockThreadid)
	{
		++_writeCount;
		return;
	}

	// 아무도 소유 및 공유하고 있지 않을 때, 경합해서 소유권을 얻는다.
	const int64 beginTick = GetTickCount64();
	const uint32 desired = ((LThreadid << 16) & WRITE_THREAD_MASK);

	while (true)
	{
		// 이 포문은 내부적으로 알 수 있는 것은 리드카운트가 다른 값이라면 
		// 무조건 실패를 하게되고 성공할 때 까지 계속 시도하게 된다.
		// Read도 없고 Write도 없는 꺠끗한 상태일 떄만 내가 락을 잡을 수 있다.
		for (uint32 SpinCount = 0; SpinCount < MAX_SPIN_COUNT; ++SpinCount)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(expected, desired))
			{
				++_writeCount; // 따로 관리하는 이유 : 재귀적으로 사용할려고
				return;
			}

		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Lock_TimeOut");

		this_thread::yield();
	}


}

void Lock::WriteUnLock()
{
	//ReadLock 다 풀기 전에는 WriteUnlock은 불가능하다.
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_OREDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공.
	const uint32 lockThreadid = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

	if (LThreadid == lockThreadid)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 올린다.
	const int64 beginTick = GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Lock_TimeOut");

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
