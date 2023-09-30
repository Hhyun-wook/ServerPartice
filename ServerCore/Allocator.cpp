#include "pch.h"
#include "Allocator.h"

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	return ::free(ptr);
}

// ���� ���� ����� ����Ҷ��� ���������� , �޸𸮿����� ������ �� �־�
// ����Ѵ�.
void* StompAllocator::Alloc(int32 size)
{
	// 4������ 4096kb -> 4+ 4095 = 4099 �ϱ�
	// 4095 + 4096 = 1.9999 -> �ݿø��ڵ��̴�.
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount * PAGE_SIZE - size;


	void *baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
	// Ȯ���ϰ� 1����Ʈ�� �ٲ��� 
	// ĳ���������������� 1����Ʈ�� ����������
}

void StompAllocator::Release(void* ptr)
{
	//[         [ptr]]  // �ٽ� �����ִ� �������� �ּҸ� �˾ƾߵȴ�.
	//0x1000

	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);

}
