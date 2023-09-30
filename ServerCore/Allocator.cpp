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

// 아주 작은 사이즈를 사용할때는 단점이지만 , 메모리오염을 예방할 수 있어
// 사용한다.
void* StompAllocator::Alloc(int32 size)
{
	// 4였으면 4096kb -> 4+ 4095 = 4099 니까
	// 4095 + 4096 = 1.9999 -> 반올림코드이다.
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount * PAGE_SIZE - size;


	void *baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
	// 확실하게 1바이트로 바꿔줌 
	// 캐스팅해주지않으면 1바이트로 나오지않음
}

void StompAllocator::Release(void* ptr)
{
	//[         [ptr]]  // 다시 위에있는 포인터의 주소를 알아야된다.
	//0x1000

	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);

}
