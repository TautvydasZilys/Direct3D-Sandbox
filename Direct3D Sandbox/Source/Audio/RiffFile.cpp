#include "PrecompiledHeader.h"
#include "RiffFile.h"
#include "Tools.h"

RiffFile::RiffFile(int format) :
	fileFormat(format)
{
}

RiffFile::~RiffFile()
{
}

RiffFile::RiffFile(RiffFile&& other) :
	fileFormat(other.fileFormat),
	chunks(std::move(other.chunks))
{
}

void RiffFile::AddChunk(int fourCC, int size, const uint8_t* data)
{
	Assert(chunks.find(*reinterpret_cast<FourCCWrapper*>(&fourCC)) == chunks.end());
	chunks.emplace(fourCC, RiffChunk(size, data));
}

RiffFile RiffFile::Create(const wstring& filePath)
{
	unsigned int dataSize, fileType;
	const auto riffData = Tools::ReadFileToVector(filePath);

#if DEBUG
	unsigned int bufferPosition = 0;
	auto fileFourCC = Tools::BufferReader::ReadUInt(riffData, bufferPosition);
	Assert(fileFourCC = RiffFourCC::RIFF);
#else
	unsigned int bufferPosition = 4;
#endif

	dataSize = Tools::BufferReader::ReadUInt(riffData, bufferPosition);
	fileType = Tools::BufferReader::ReadUInt(riffData, bufferPosition);

	RiffFile riff(fileType);

	while (bufferPosition < dataSize + 8)
	{
		auto chunkFourCC = Tools::BufferReader::ReadUInt(riffData, bufferPosition);
		Assert(chunkFourCC != RiffFourCC::LIST);	// Lists currently not supported

		auto chunkSize = Tools::BufferReader::ReadUInt(riffData, bufferPosition);
		
		riff.AddChunk(chunkFourCC, chunkSize, &riffData[bufferPosition]); 
		bufferPosition += chunkSize + (chunkSize % 2);
	}

	return riff;
}


RiffChunk& RiffFile::GetChunk(int fourCC)
{
	auto chunk = chunks.find(*reinterpret_cast<FourCCWrapper*>(&fourCC));
	Assert(chunk != chunks.end());

	return chunk->second;
}