#pragma once

#include "Types.h"
#include "MemoryPool.h"

/*
	ObjectPool

	Memory Pool 보다는 작은 개념
	
	서로 공유해서 memoryPool을 사용하지만 어떤 클래스에서 문제가
	생겼는지 알수가없다. 그래서 경우에 따라서 특정클래스들을 관리하기위해
	ObjectPool을 만든다.
*/

template <typename Type>
class ObjectPool
{

public:
	template <typename ...Args>
	static Type* Pop(Args&& ...args)
	{
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize);)
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif
		// 생성시 POP()
		new(memory)Type(forward<Args>(args)...); //place New 생성자 호출방법
		return memory;
	}

	static void Push(Type* obj)
	{


		// 반납할때 Push
		obj->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif

		
	
	}

	static shared_ptr<Type> MakeShared()
	{
		shared_ptr<Type> ptr = { Pop(),Push };
		return ptr;
	}

	

private:
	static int32		 s_allocSize;
	static MemoryPool	 s_pool;

};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };