#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"


/*
	Memory( 메모리풀 ,또는 메모리관련을 관리하는 클래스)
*/
Memory::Memory()
{
	int32 size = 0;
	int32 tableIndex = 0;

	// 1024까지 32
	for (size = 32; size <= 1024; size += 32)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}

	// 2048까지 128
	for (; size <= 2048; size += 128)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}

	// 4096까지 256
	for (; size <= 4096; size += 256)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}
}

Memory::~Memory()
{
	for (MemoryPool* pool : _pools)
		delete pool;

	_pools.clear();
}

void* Memory::Allocate(int32 size)
{
	// 공용데이터를 Write하지않기 떄문에 락을 걸지않는다.
	// 어차피 Push, Pop에서 락을해서 동기화를 진행하기도한다.
	MemoryHeader* header = nullptr;

	const int32 allocSize = size + sizeof(MemoryHeader);

	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링의 최대 크기를 벗어나면 일반 할당
		header = reinterpret_cast<MemoryHeader*>(::malloc(allocSize));
	}
	else
	{
		// 메모리 풀에서 꺼내온다.
		header = _poolTable[allocSize]->Pop();
	}
	//몇바이트를 할당했는지 정보
    return MemoryHeader::AttachHeader(header,allocSize);
}

void Memory::Relase(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 큰 크기는 일반할당 하니까 일반해제 한다.
		::free(header);
	}
	else
	{
		//메모리풀에 반납
		_poolTable[allocSize]->Push(header);
	}

}
