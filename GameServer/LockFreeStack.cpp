#include "pch.h"
#include "LockFreeStack.h"

/*
	싱글쓰레드에서만 돌아가는 방법
*/
//
//void InitializeHead(SListHeader* header)
//{
//	header->next = nullptr;
//}
//
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	entry->next = header->next;
//	header->next = entry;
//}
//
//SListEntry* PopEntrySList(SListHeader* header)
//{
//	SListEntry* first = header->next;
//
//	if (first != nullptr)
//		header->next = first->next;
//
//	return first;
//}

/*
	멀티쓰레드에서 돌아가는 방법 2차시도 : ABA 문제가 생길수 있음
*/
//void InitializeHead(SListHeader* header)
//{
//	header->next = nullptr;
//}
//
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	entry->next = header->next;
//	
//	while (::InterlockedCompareExchange64((int64*)&header->next, 
//		(int64)entry, (int64)entry->next) ==0)
//	{
//
//	}
//
//	header->next = entry;
//}
//
//// [data][][] ->List
//// Header [next]
//SListEntry* PopEntrySList(SListHeader* header) 
//{
//	SListEntry* expected = header->next;
//
//	// CAS 를 사용할 떄 ABA문제를 생각해야한다.
//	// 이 때 해결방법은 비교대상을 1개가아닌 2개로 사용하는것이다.
//	// ABA problem
//	// A에서 B가 됐다가 다시 A가 되는 문제
//	
//	// 만약에 Header 가 5000이라면 Header에다 6000을 넣어줘
//	// [5000]->[6000]->[7000]
//	// [Header]
//
//	
//
//	while (expected && ::InterlockedCompareExchange64((int64*)&header->next,
//		(int64)expected->next, (int64)expected) == 0)
//	{
//
//	}
//
//
//	return expected;
//}


/*
	3차시도
*/
//void InitializeHead(SListHeader* header)
//{
//	header->alignment = 0;
//	header->region = 0;
//}
//
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	SListHeader expected = {};
//	SListHeader desired = {};
//
//	// 16바이트 정렬은 마지막 4개의 비트가 0이다.
//	desired.HeaderX64.next = (((uint64)(entry)) >> 4);
//
//	while (true)
//	{
//		expected = *header;
//
//		// 이 사이에 데이터가 변경될 수 있다.
//		entry->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
//		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
//		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;
//
//		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
//			break;
//	}
//
//}
//
//// [][] 
//// Header [next]
//SListEntry* PopEntrySList(SListHeader* header) 
//{
//	SListHeader expected = {};
//	SListHeader desired = {};
//	SListEntry* entry = nullptr;
//
//	while (true)
//	{
//		expected = *header;
//		
//		entry = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);
//
//		if (entry == nullptr)
//			break;
//
//		// Use- After -Free 문제가 생길 수 있다.
//		desired.HeaderX64.next = ((uint64)entry->next) >> 4;
//		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;
//		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;
//
//		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
//			break;
//	}
//
//	return entry;
//}