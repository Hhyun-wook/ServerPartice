#pragma once


/*
	LockFreeStack 1��° ��� // �̱� �����忡���� �ߵ��ư����� ��Ƽ������� ��

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
	 2��° ��� : ��Ƽ�����忡�� �����ϴ� LockFreeStack
	 ABA ������ �߻� : ->a���� b�� ���ٰ� �ٽ� a�� ���°� �̶� ������ ��ȭ�� �����������Ѵ�.
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
	3�� ��� : ABA������ �ذ��ϰ� ��Ƽ�����忡�� �����ϴ� LockFreeStack
*/

DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;

};

// [data][][] ->List
// [Header][next]
DECLSPEC_ALIGN(16) //�ڵ������� 16����Ʈ�� ����
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
			uint64 depth = 16;		//��Ʈ������ �ɰ����°� 16��Ʈ�� �ɰ�����.
			uint64 sequence = 48;	//��Ʈ������ �ɰ����°� 48��Ʈ�� �ɰ�����.
			uint64 reserved = 4;
			uint64 next = 60;
		}HeaderX64;

	};

	SListEntry* next = nullptr;

};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
