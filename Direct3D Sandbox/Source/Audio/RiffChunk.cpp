#include "PrecompiledHeader.h"
#include "RiffChunk.h"


RiffChunk::RiffChunk(int size, const uint8_t* data) :
	size(size), data(new uint8_t[size])
{
	memcpy(this->data.get(), data, size);
}

RiffChunk::RiffChunk(RiffChunk&& other) :
	size(other.size),
	data(std::move(other.data))
{
}

RiffChunk::~RiffChunk()
{
}
