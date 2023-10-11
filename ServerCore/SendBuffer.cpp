#include "pch.h"
#include "SendBuffer.h"

/*
	SendBuffer
*/

SendBuffer::SendBuffer(SendBufferChunkRef owner, BYTE* buffer, int32 allocSize)
	:_owner{owner},_buffer{buffer},_allocSize(allocSize)
{
	
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 writeSize)
{
	ASSERT_CRASH(_allocSize >= writeSize);
	
	_writeSize = writeSize;
	_owner->Close(writeSize);
}




/*
	SendBufferChunk
*/
SendBufferChunk::SendBufferChunk()
{
}

SendBufferChunk::~SendBufferChunk()
{
}

void SendBufferChunk::Reset()
{
	_open = false;
	_usedSize = 0;
}

SendBufferRef SendBufferChunk::Open(uint32 allocSize)
{
	ASSERT_CRASH(allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT_CRASH(_open == false);

	if (allocSize > FreeSize())
		return nullptr;

	_open = true;

	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(),Buffer(),allocSize);
}

void SendBufferChunk::Close(uint32 writeSize)
{
	ASSERT_CRASH(_open == true);
	_open = false;
	_usedSize += writeSize;
}
  

/*
	SendBufferManager 
*/
// 큼지막한 버퍼에서 우리가 사용할 영역을 열어준다는 개념 
// 다 사용하는 건 아니고 close한 영역까지 사용한다는 개념
SendBufferRef SendBufferManager::Open(uint32 size)
{
	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = Pop();		// 락을 건다. (할당받지않았기 때문에 
										//  공용데이터인 _sendBufferChunks 새로운 것을 할당 받아야되므로 
		LSendBufferChunk->Reset();		
		
	}
	ASSERT_CRASH(LSendBufferChunk->IsOpen() == false);

	// 다썼으면 버리고 새걸로 교체
	if (LSendBufferChunk->FreeSize() < size)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	cout << "Free" << LSendBufferChunk->FreeSize() << endl;


	return LSendBufferChunk->Open(size);
}

SendBufferChunkRef SendBufferManager::Pop()
{
	{
		WRITE_LOCK;
		if (_sendBufferChunks.empty() == false)
		{
			SendBufferChunkRef sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return SendBufferChunkRef(xnew<SendBufferChunk>(),PushGlobal);
}

void SendBufferManager::Push(SendBufferChunkRef buffer)
{
	WRITE_LOCK;
	_sendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	GSendBufferManager->Push(SendBufferChunkRef(buffer, PushGlobal));
}

