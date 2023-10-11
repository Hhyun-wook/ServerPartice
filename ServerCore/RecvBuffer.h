#pragma once

/*
	RecvBuffer
*/


// 초기위치					: [_readPos,_writePos ][][][][][][][]
// 데이터를 쓴 다음에 위치	: [_readPos ][_writePos][][][][][][]
// 근데 어느순간에 꽉차있는 경우가 있다. 이때 전통적인 방법은 순환방법(ex)원형큐) 처럼만드는 것이다.
// 하지만 끝부분을 처리하는게 좀 까다롭다. 그래서 공간을 여유있게 만든 후 read write가 겹친 경우(데이터가 꽉찬경우)
// read write를 0번위치로 바꾼다. 근데 데이터가 남아있다면 0번위치에 복사를 한 후에 커서위치를 다시 이동한다.
class RecvBuffer
{
	enum {BUFFER_COUNT =10,}; //훨씬더 크게 버퍼를 잡아준다. => Read Write는 버퍼공간이 클수록 겹칠확률이 크다.


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
	int32			_readPos = 0;		// 일종의 커서
	int32			_writePos = 0;	// 일종의 커서
	xVector<BYTE>	_buffer;
};

