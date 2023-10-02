#pragma once

enum
{
	SLIST_ALIGNMENT =16,
};

/*
	MemoryHeader
*/

// 메모리풀을 하는 이유 -> 메모리 (파편화)단편화를 예방하기 위해서
//	[32][64][]	// 각각 크기에 따른 풀을 가지고 있는다.
// [    /    ] -> 크기에 따라서 쪼개서 사용할 수 있는 방법이 있다.
// [32 32 32 32]  -> 동일한 크기로 관리하는게 편하다. 

// 메모리풀링이랑 StompAllocator는 궁합이 안맞는다.
// StompAllocator의 기본개념은 메모리가 필요없다면 운영체제에 부탁해서
//              그 메모리를 날려서 접근을 못하게하는 정책이기 때문이다.

// 그래서 상황에 따라 어떤것을 사용해야할지 고민해야한다.

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader :public SLIST_ENTRY
{
	// [MemoryHeader][Data] // 표준 Allocator 객체도 이런식으로 구성되어있다.
	MemoryHeader(int32 size) : allocSize(size){}

	
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		// [MemoryHeader][Data]  [Data] 부분 리턴

		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		// [MemoryHeader][Data]  [MemoryHeader] 부분을 가져와서 리턴
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;

};

/*
	MemoryPool
*/

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();


private:
	SLIST_HEADER _header;	// 첫번째 데이터를 가르킨다.
	int32 _allocSize = 0;
	atomic<int32> _allocCount = 0;

};

