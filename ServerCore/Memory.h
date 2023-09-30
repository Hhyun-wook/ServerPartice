#pragma once

#include "Allocator.h"

template<typename Type, typename ...Args>
Type* xnew(Args&& ...args) // c++11���� ������ ���� : ���׵� ���ø�����
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	//placement new 
	// �̹� �޸𸮴� �����ϱ� �޸������� �����ڸ� ȣ���ش޶�� ����
	new(memory) Type(std::forward<Args>(args)...);  // c++11���� ������ ���� : ���׵� ���ø�����

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	BaseAllocator::Release(obj);
}
