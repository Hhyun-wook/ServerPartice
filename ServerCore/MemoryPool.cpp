#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize)
	:_allocSize(allocSize)
{
}

MemoryPool::~MemoryPool()
{
	while (!_queue.empty())
	{
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	};
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	ptr->allocSize = 0;

	// Pool에 메모리 반납
	_queue.push(ptr);

	_allocCount.fetch_add(1);

}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;

	{
		WRITE_LOCK;

		//Pool에 여분의 데이터가 있는가?
		if (!_queue.empty())
		{
			// 있으면 꺼낸다.
			header = _queue.front();
			_queue.pop();
		}
	}

	// 없으면 새로 만든다.
	{
		if (header == nullptr)
		{
			header = reinterpret_cast<MemoryHeader*> (::malloc(_allocSize));
		}
		else
		{
			ASSERT_CRASH(header->allocSize == 0);
		}
	}

	_allocCount.fetch_add(1);
	
	return header;
}
