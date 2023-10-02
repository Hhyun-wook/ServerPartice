#pragma once


/*
	LockFreeStack 1번째 방법 // 싱글 스레드에서는 잘돌아가지만 멀티쓰레드는 ㄴ

*/
//
//struct SListEntry
//{
//	SListEntry* next;
//
//};
//
//
//// [data][][] ->List
//// [Header][next]
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//
//};
//
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

/*
	 2번째 방법 : 멀티쓰레드에서 동작하는 LockFreeStack
	 ABA 문제가 발생 : ->a에서 b로 갔다가 다시 a로 가는것 이때 문제는 변화를 감지하지못한다.
*/

//struct SListEntry
//{
//	SListEntry* next;
//
//};
//
//// [data][][] ->List
//// [Header][next]
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//
//};
//
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);


/*
	3차 방법 : ABA문제를 해결하고 멀티쓰레드에서 동작하는 LockFreeStack
*/

DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;

};

// [data][][] ->List
// [Header][next]
DECLSPEC_ALIGN(16) //자동적으로 16바이트로 정렬
struct SListHeader
{
	SListHeader()
	{
		alignment = 0;
		region = 0;
	}

	union
	{
		struct
		{
			uint64 alignment;
			uint64 region;
		}DUMMYSTRUCTNAME;
		struct
		{
			uint64 depth = 16;		//비트단위로 쪼개쓰는것 16비트로 쪼개쓴다.
			uint64 sequence = 48;	//비트단위로 쪼개쓰는것 48비트로 쪼개쓴다.
			uint64 reserved = 4;
			uint64 next = 60;
		}HeaderX64;

	};

	SListEntry* next = nullptr;

};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
