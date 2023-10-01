#pragma once

/*
	BaseAllocator
*/

class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);

};

/*
	StompAllocator
*/
// 버그를잡는데 유용한 클래스(메모리 오염문제)
// 가상메모리의 기본 
// 댕글리포인터를 쉽게 잡을 수 있다.
// 하지만 메모리 오버플로우 문제를 잡을 수는 없다.
// 실제로 할당된 영역이 실제 필요한 영역보다 클 수 있다.
class StompAllocator
{
	enum {PAGE_SIZE =0x1000};

public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);

};

/*
	MemoryPoolAllocator
*/

class PoolAllocator
{

public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);

};



/*
	STL_Allocator
*/
template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(xxalloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		xxrelease(ptr);
	}

};


