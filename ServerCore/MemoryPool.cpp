#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize)
	:_allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	// ==�� �ƴϰ� = nullptr üũ�ϴ� ������
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
	{
		::_aligned_free(memory);
	};
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	
	ptr->allocSize = 0;

	// Pool�� �޸� �ݳ�
	::InterlockedPushEntrySList(&_header,static_cast<SLIST_ENTRY*>(ptr));

	_allocCount.fetch_add(1);

}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// ������ ���� �����.
	{
		if (memory == nullptr)
		{
			memory = reinterpret_cast<MemoryHeader*> (::_aligned_malloc(_allocSize,SLIST_ALIGNMENT));
		}
		else
		{
			ASSERT_CRASH(memory->allocSize == 0);
		}
	}

	_allocCount.fetch_add(1);
	
	return memory;
}