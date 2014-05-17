#pragma once

class RiffChunk
{
private:
	int size;
	unique_ptr<uint8_t[]> data;
	
	RiffChunk(const RiffChunk& other);

public:
	RiffChunk(int size, const uint8_t* data);
	RiffChunk(RiffChunk&& other);
	~RiffChunk();

	inline int GetSize() const { return size; }
	inline const uint8_t* GetData() const { return data.get(); }
	inline unique_ptr<uint8_t[]>& GetDataBuffer() { return data; }
};