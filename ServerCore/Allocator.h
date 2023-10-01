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
// ���׸���µ� ������ Ŭ����(�޸� ��������)
// ����޸��� �⺻ 
// ��۸������͸� ���� ���� �� �ִ�.
// ������ �޸� �����÷ο� ������ ���� ���� ����.
// ������ �Ҵ�� ������ ���� �ʿ��� �������� Ŭ �� �ִ�.
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


