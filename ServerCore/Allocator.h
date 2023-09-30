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



