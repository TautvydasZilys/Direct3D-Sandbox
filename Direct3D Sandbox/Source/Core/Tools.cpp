#include "PrecompiledHeader.h"
#include "Tools.h"

inline static long long int InitPerformanceCounterFrequency()
{
	long long int frequency;

	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	return frequency;
}

long long int s_PerformanceCounterFrequency = InitPerformanceCounterFrequency();

float Tools::GetTime()
{
	long long int timer;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&timer));
	return static_cast<float>(timer) / static_cast<float>(m_PerformanceCounterFrequency);
}

vector<uint8_t> Tools::ReadFileToVector(wstring path)
{
	ifstream in(path, ios::binary);
	Assert(in.is_open());
	
	in.seekg(0, ios::end);
	int fileLength = static_cast<int>(in.tellg());
	vector<uint8_t> fileContents(fileLength);
	in.seekg(0, ios::beg);

	in.read(reinterpret_cast<char*>(&fileContents[0]), fileLength);
	in.close();

	return fileContents;
}