#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{	
	
	/*if (_lockFlag == EMPTY_FLAG)
	{
		const uint32 desired = ((LThreadid << 16) & WRITE_THREAD_MAX);
		_lockFlag = desired;
	}*/
	// ������ �����尡 �����ϰ� �ִٸ� ������ �����ϰ� �����.
	const uint32 lockThreadid = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

	if (LThreadid == lockThreadid)
	{
		++_writeCount;
		return;
	}

	// �ƹ��� ���� �� �����ϰ� ���� ���� ��, �����ؼ� �������� ��´�.
	const int64 beginTick = GetTickCount64();
	const uint32 desired = ((LThreadid << 16) & WRITE_THREAD_MASK);

	while (true)
	{
		// �� ������ ���������� �� �� �ִ� ���� ����ī��Ʈ�� �ٸ� ���̶�� 
		// ������ ���и� �ϰԵǰ� ������ �� ���� ��� �õ��ϰ� �ȴ�.
		// Read�� ���� Write�� ���� �Ʋ��� ������ ���� ���� ���� ���� �� �ִ�.
		for (uint32 SpinCount = 0; SpinCount < MAX_SPIN_COUNT; ++SpinCount)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(expected, desired))
			{
				++_writeCount; // ���� �����ϴ� ���� : ��������� ����ҷ���
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
	//ReadLock �� Ǯ�� ������ WriteUnlock�� �Ұ����ϴ�.
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_OREDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����.
	const uint32 lockThreadid = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

	if (LThreadid == lockThreadid)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// �ƹ��� �����ϰ� ���� ���� �� �����ؼ� ���� ī��Ʈ�� �ø���.
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
