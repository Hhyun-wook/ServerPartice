#pragma once

class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE* Buffer() { return _buffer; }
	uint32  Size() { return _size; }
	uint32 WriteSize() { return _pos; }
	uint32 FreeSize() { return _size - _pos; }



	template <typename T>
	bool Write(T* sour) { return Write(sour, sizeof(T)); }
	bool Write(void* sour, uint32 len);

	template<typename T>
	T* Reserve();

	template<typename T>
	BufferWriter& operator <<(OUT T& sour);

	template<typename T>
	BufferWriter& operator <<(OUT T&& sour);

private:
	BYTE* _buffer = nullptr;
	uint32	_size = 0;
	uint32	_pos = 0;
};

template<typename T>
inline T* BufferWriter::Reserve()
{
	if (FreeSize() < sizeof(T))
		return nullptr;

	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return ret;
}

template<typename T>
inline BufferWriter& BufferWriter::operator<<(OUT T& sour)
{
	*reinterpret_cast<T*>(&_buffer[_pos]) = sour;
	_pos += sizeof(T);
	return *this;

}

template<typename T>
inline BufferWriter& BufferWriter::operator<<(OUT T&& sour)
{
	*reinterpret_cast<T*>(&_buffer[_pos]) = std::move(sour);
	_pos += sizeof(T);
	return *this;
}
