#pragma once

#include "Allocator.h"

class MemoryPool;

/*
	Memory�� �����ϴ� Ŭ����
*/

class Memory
{    // ���� �������� �޸𸮸� ���� ����ϱ� ������ 
	 // �޸𸮰� ���� �ֵ��� ���ַ� �����.
	enum
	{
		//~1024���� 32���� , ~2048���� 128, ~4096���� 256
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE =4096,
		
	};

public:
	Memory();
	~Memory();

	void* Allocate(int32 size);
	void Relase(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// ���������� �޸� ũ�� <-> �޸� Ǯ
	// �ð����⵵�� ����ð��ȿ� ã������ �����̺��� ������ش�.
	
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];



};


template<typename Type, typename ...Args>
Type* xnew(Args&& ...args) // c++11���� ������ ���� : ���׵� ���ø�����
{
	//Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));

	//placement new 
	// �̹� �޸𸮴� �����ϱ� �޸������� �����ڸ� ȣ���ش޶�� ����
	new(memory) Type(std::forward<Args>(args)...);  // c++11���� ������ ���� : ���׵� ���ø�����

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	//BaseAllocator::Release(obj);
	PoolAllocator::Release(obj);
}


template<typename Type>
shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>(xnew<Type>(), xdelete<Type>);
}