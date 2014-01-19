#include "PrecompiledHeader.h"
#include "Tools.h"

long long int Tools::m_PerformanceCounterFrequency;

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