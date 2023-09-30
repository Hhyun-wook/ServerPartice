#pragma once

#include "Allocator.h"

template<typename Type, typename ...Args>
Type* xnew(Args&& ...args) // c++11부터 등장한 문법 : 베네딕 템플릿문법
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	//placement new 
	// 이미 메모리는 있으니까 메모리위에다 생성자를 호출해달라는 문법
	new(memory) Type(std::forward<Args>(args)...);  // c++11부터 등장한 문법 : 베네딕 템플릿문법

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	BaseAllocator::Release(obj);
}
