#pragma once

/*
	MemoryHeader
*/

// �޸�Ǯ�� �ϴ� ���� -> �޸� (����ȭ)����ȭ�� �����ϱ� ���ؼ�
//	[32][64][]	// ���� ũ�⿡ ���� Ǯ�� ������ �ִ´�.
// [    /    ] -> ũ�⿡ ���� �ɰ��� ����� �� �ִ� ����� �ִ�.
// [32 32 32 32]  -> ������ ũ��� �����ϴ°� ���ϴ�. 

// �޸�Ǯ���̶� StompAllocator�� ������ �ȸ´´�.
// StompAllocator�� �⺻������ �޸𸮰� �ʿ���ٸ� �ü���� ��Ź�ؼ�
//              �� �޸𸮸� ������ ������ ���ϰ��ϴ� ��å�̱� �����̴�.

// �׷��� ��Ȳ�� ���� ����� ����ؾ����� ����ؾ��Ѵ�.

struct MemoryHeader
{
	// [MemoryHeader][Data] // ǥ�� Allocator ��ü�� �̷������� �����Ǿ��ִ�.
	MemoryHeader(int32 size) : allocSize(size){}

	
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		// [MemoryHeader][Data]  [Data] �κ� ����

		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		// [MemoryHeader][Data]  [MemoryHeader] �κ��� �����ͼ� ����
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;

};

/*
	MemoryPool
*/
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();


private:
	int32 _allocSize = 0;
	atomic<int32> _allocCount = 0;

	USE_LOCK;
	queue<MemoryHeader*> _queue;
};

