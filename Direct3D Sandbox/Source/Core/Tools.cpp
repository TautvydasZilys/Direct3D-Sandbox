#include "PrecompiledHeader.h"
#include "Parameters.h"
#include "Tools.h"

inline static long long int InitPerformanceCounterFrequency()
{
	long long int frequency;

	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	return frequency;
}

long long int s_PerformanceCounterFrequency = InitPerformanceCounterFrequency();

double Tools::GetTime()
{
	long long int timer;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&timer));
	return static_cast<double>(timer) / static_cast<double>(s_PerformanceCounterFrequency);
}

vector<uint8_t> Tools::ReadFileToVector(const wstring& path)
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


ModelData Tools::LoadModel(const wstring& path, bool shouldInvert)
{
	ModelData model;
	auto modelPath = path;

	if (shouldInvert)
	{
		modelPath = modelPath.insert(modelPath.find_last_of(L'.'), L"_inverted");
	}

	ifstream in(modelPath, ios::binary);
	Assert(in.is_open());
	
	in.read(reinterpret_cast<char*>(&model.vertexCount), sizeof(int));
	model.vertices = unique_ptr<VertexParameters[]>(new VertexParameters[model.vertexCount]);
	in.read(reinterpret_cast<char*>(model.vertices.get()), model.vertexCount * sizeof(VertexParameters));

	in.read(reinterpret_cast<char*>(&model.indexCount), sizeof(int));
	model.indices = unique_ptr<unsigned int[]>(new unsigned int[model.indexCount]);
	in.read(reinterpret_cast<char*>(model.indices.get()), model.indexCount * sizeof(unsigned int));

	OutputDebugString((L"Loading model from " + path + L":\r\n").c_str());
	OutputDebugString((L"\tNumber of vertices: " + to_wstring(model.vertexCount) + L"\r\n").c_str());
	OutputDebugString((L"\tNumber of indices: " + to_wstring(model.indexCount) + L"\r\n").c_str());

	in.close();
	return model;
}

vector<wstring> Tools::GetFilesInDirectory(wstring path, const wstring& searchPattern, bool recursive)
{
	vector<wstring> result;
	WIN32_FIND_DATA findData;

	if (path[path.length() - 1] != L'\\')
	{
		path += L'\\';
	}
		
	auto fileName = path + searchPattern;
	auto searchHandle = FindFirstFileEx(fileName.c_str(), FindExInfoStandard, &findData, FindExSearchNameMatch, nullptr, 0);
	Assert(searchHandle != INVALID_HANDLE_VALUE);

	do
	{
		if (_wcsicmp(findData.cFileName, L".") != 0 && _wcsicmp(findData.cFileName, L"..") != 0)
		{
			if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (recursive)
				{
					for (const auto& file : GetFilesInDirectory(path + findData.cFileName, searchPattern, recursive))
					{
						result.emplace_back(file);
					}
				}
			}
			else
			{
				result.emplace_back(path + findData.cFileName);
			}
		}

		FindNextFile(searchHandle, &findData);
	}
	while (GetLastError() != ERROR_NO_MORE_FILES);

	FindClose(searchHandle);
	
	SetLastError(ERROR_SUCCESS);
	return result;
}

bool Tools::DirectoryExists(const wstring& path)
{
	WIN32_FILE_ATTRIBUTE_DATA attributes;

	auto result = GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &attributes);
	Assert(result != 0 || GetLastError() == ERROR_FILE_NOT_FOUND);

	return (attributes.dwFileAttributes != INVALID_FILE_ATTRIBUTES) && (attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

int Tools::GetMemoryUsage()
{
#if !WINDOWS_PHONE
	PROCESS_MEMORY_COUNTERS memoryInfo;
	auto myProcess = GetCurrentProcess();
	auto result = GetProcessMemoryInfo(myProcess, &memoryInfo, sizeof(memoryInfo));
	
	Assert(result != 0);

	return static_cast<int>(memoryInfo.WorkingSetSize / (1024 * 1024));
#else
	return static_cast<int>(Windows::Phone::System::Memory::MemoryManager::ProcessCommittedBytes / (1024 * 1024));
#endif
}

string Tools::ToLower(const string& str)
{
	string lowerStr;
	lowerStr.resize(str.length());

	transform(begin(str), end(str), begin(lowerStr), ::tolower);
	return lowerStr;
}

wstring Tools::ToLower(const wstring& str)
{
	wstring lowerStr;
	lowerStr.resize(str.length());

	transform(begin(str), end(str), begin(lowerStr), ::towlower);
	return lowerStr;
}

string Tools::BufferReader::ReadString(const vector<uint8_t>& buffer, unsigned int& position)
{
	string str = reinterpret_cast<const char*>(&buffer[position]);
	position += static_cast<unsigned int>(str.length()) + 1;
	return str;
}

unsigned int Tools::BufferReader::ReadUInt(const vector<uint8_t>& buffer, unsigned int& position)
{
	auto value = *reinterpret_cast<const unsigned int*>(&buffer[position]);
	position += 4;
	return value;
}

float Tools::BufferReader::ReadFloat(const vector<uint8_t>& buffer, unsigned int& position)
{
	auto value = *reinterpret_cast<const float*>(&buffer[position]);
	position += 4;
	return value;
}

char Tools::BufferReader::ReadChar(const vector<uint8_t>& buffer, unsigned int& position)
{
	auto value = *reinterpret_cast<const char*>(&buffer[position]);
	position++;
	return value;
}
