#pragma once

/*
	RecvBuffer
*/


// �ʱ���ġ					: [_readPos,_writePos ][][][][][][][]
// �����͸� �� ������ ��ġ	: [_readPos ][_writePos][][][][][][]
// �ٵ� ��������� �����ִ� ��찡 �ִ�. �̶� �������� ����� ��ȯ���(ex)����ť) ó������� ���̴�.
// ������ ���κ��� ó���ϴ°� �� ��ٷӴ�. �׷��� ������ �����ְ� ���� �� read write�� ��ģ ���(�����Ͱ� �������)
// read write�� 0����ġ�� �ٲ۴�. �ٵ� �����Ͱ� �����ִٸ� 0����ġ�� ���縦 �� �Ŀ� Ŀ����ġ�� �ٽ� �̵��Ѵ�.
class RecvBuffer
{
	enum {BUFFER_COUNT =10,}; //�ξ��� ũ�� ���۸� ����ش�. => Read Write�� ���۰����� Ŭ���� ��ĥȮ���� ũ��.


public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	int32			DataSize() { return _writePos - _readPos; }
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0;
	int32			_bufferSize = 0;
	int32			_readPos = 0;		// ������ Ŀ��
	int32			_writePos = 0;	// ������ Ŀ��
	xVector<BYTE>	_buffer;
};

