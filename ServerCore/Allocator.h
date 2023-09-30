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
// ���׸���µ� ������ Ŭ����(�޸� ��������)
// ����޸��� �⺻ 
// ��۸������͸� ���� ���� �� �ִ�.
// ������ �޸� �����÷ο� ������ ���� ���� ����.
// ������ �Ҵ�� ������ ���� �ʿ��� �������� Ŭ �� �ִ�.
class StompAllocator
{
	enum {PAGE_SIZE =0x1000};

public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);

};
