#pragma once

#include "Allocator.h"

class MemoryPool;

/*
	Memory를 관리하는 클래스
*/

class Memory
{    // 작은 사이즈의 메모리를 자주 사용하기 때문에 
	 // 메모리가 작은 애들을 위주로 만든다.
	enum
	{
		//~1024까지 32단위 , ~2048까지 128, ~4096까지 256
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

	// 최종적으로 메모리 크기 <-> 메모리 풀
	// 시간복잡도를 상수시간안에 찾기위한 테이이블을 만들어준다.
	
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];



};


template<typename Type, typename ...Args>
Type* xnew(Args&& ...args) // c++11부터 등장한 문법 : 베네딕 템플릿문법
{
	//Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));

	//placement new 
	// 이미 메모리는 있으니까 메모리위에다 생성자를 호출해달라는 문법
	new(memory) Type(std::forward<Args>(args)...);  // c++11부터 등장한 문법 : 베네딕 템플릿문법

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