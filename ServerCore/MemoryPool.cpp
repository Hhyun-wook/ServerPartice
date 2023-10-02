#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize)
	:_allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	// ==이 아니고 = nullptr 체크하는 패턴임
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
	{
		::_aligned_free(memory);
	};
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	
	ptr->allocSize = 0;

	// Pool에 메모리 반납
	::InterlockedPushEntrySList(&_header,static_cast<SLIST_ENTRY*>(ptr));

	_allocCount.fetch_add(1);

}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// 없으면 새로 만든다.
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
