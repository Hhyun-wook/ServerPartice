#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"


/*
	Memory( �޸�Ǯ ,�Ǵ� �޸𸮰����� �����ϴ� Ŭ����)
*/
Memory::Memory()
{
	int32 size = 0;
	int32 tableIndex = 0;

	// 1024���� 32
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

	// 2048���� 128
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

	// 4096���� 256
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
	// ���뵥���͸� Write�����ʱ� ������ ���� �����ʴ´�.
	// ������ Push, Pop���� �����ؼ� ����ȭ�� �����ϱ⵵�Ѵ�.
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else

	if (allocSize > MAX_ALLOC_SIZE)
	{
		// �޸� Ǯ���� �ִ� ũ�⸦ ����� �Ϲ� �Ҵ�
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		// �޸� Ǯ���� �����´�.
		header = _poolTable[allocSize]->Pop();
	}
#endif


	//�����Ʈ�� �Ҵ��ߴ��� ����
    return MemoryHeader::AttachHeader(header,allocSize);
}

void Memory::Relase(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// ū ũ��� �Ϲ��Ҵ� �ϴϱ� �Ϲ����� �Ѵ�.
		::_aligned_free(header);
	}
	else
	{
		//�޸�Ǯ�� �ݳ�
		_poolTable[allocSize]->Push(header);
	}

#endif


}
