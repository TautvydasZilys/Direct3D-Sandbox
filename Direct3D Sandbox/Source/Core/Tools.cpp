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

float Tools::GetTime()
{
	long long int timer;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&timer));
	return static_cast<float>(timer) / static_cast<float>(s_PerformanceCounterFrequency);
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
	model.indices = unique_ptr<unsigned int[]>(new unsigned int[model.vertexCount]);
	in.read(reinterpret_cast<char*>(model.indices.get()), model.indexCount * sizeof(unsigned int));

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
	auto searchHandle = FindFirstFile(fileName.c_str(), &findData);
	Assert(searchHandle != INVALID_HANDLE_VALUE);

	do
	{
		if (findData.cFileName != L"." && findData.cFileName != L"..")
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

	SetLastError(ERROR_SUCCESS);
	return result;
}

bool Tools::DirectoryExists(const wstring& path)
{
	auto attributes = GetFileAttributes(path.c_str());

	return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
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
