#pragma once

#include "FourCCWrapper.h"
#include "RiffChunk.h"

enum RiffFourCC
{
	RIFF = 'FFIR',
	DATA = 'atad',
	FMT = ' tmf',
	WAVE = 'EVAW',
	LIST = 'TSIL'
};

class RiffFile
{
private:
	FourCCWrapper fileFormat;
	unordered_map<FourCCWrapper, RiffChunk> chunks;
	
	RiffFile(int fileFormat);
	RiffFile(const RiffFile& other);
	
	void AddChunk(int fourCC, int size, const uint8_t* data);

public:
	RiffFile(RiffFile&& other);
	~RiffFile();

	static RiffFile Create(const wstring& filePath);

	inline const FourCCWrapper& GetFormat() const { return fileFormat; }
	RiffChunk& GetChunk(int fourCC);
};