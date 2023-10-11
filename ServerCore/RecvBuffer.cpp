#include "pch.h"
#include "RecvBuffer.h"

/*
	RecvBuffer
*/

RecvBuffer::RecvBuffer(int32 bufferSize)
{
	_bufferSize = _capacity = bufferSize * BUFFER_COUNT;
	 
	_buffer.resize(_bufferSize);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	int32 dataSize = DataSize();
	// [][][][][][_readPos, _writePos]  쓸수있는 공간이 없을 경우
	// 처음 위치로 이동하게 만드는 것
	if (dataSize == 0)
	{
		// 마침 읽기 + 쓰기 커서가 동일한 위치라면 둘 다 리셋
		_readPos = _writePos = 0;

	}
	else // 또는 [][][][][][_readPos][][_writePos] 이경우
			// [_readPos][][_writePos][][][][][] 이렇게 만든다.
	{
		// 여유 공간이 버퍼 1개크기 미만이면 데이터를 앞으로 당긴다.
		if (FreeSize() < _bufferSize)
		{
			memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}
	}

}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;


	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;

	return true;
}
