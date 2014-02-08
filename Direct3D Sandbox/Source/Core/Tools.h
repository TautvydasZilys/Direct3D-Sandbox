#pragma once

#include "Parameters.h"
#include "PrecompiledHeader.h"

struct ModelData;

namespace Tools
{
	double GetTime();
	vector<uint8_t> ReadFileToVector(const wstring& path);
	ModelData LoadModel(const wstring& path, bool shouldInvert);

	vector<wstring> GetFilesInDirectory(wstring path, const wstring& searchPattern, bool recursive);	
	bool DirectoryExists(const wstring& path);

	string ToLower(const string& str);
	wstring ToLower(const wstring& str);

	namespace BufferReader
	{
		string ReadString(const vector<uint8_t>& buffer, unsigned int& position);
		unsigned int ReadUInt(const vector<uint8_t>& buffer, unsigned int& position);
	}
};

struct Point2D
{
	float x, y;

	Point2D() {}
	Point2D(float x, float y) : x(x), y(y) {}
};

struct ModelData
{
	unique_ptr<VertexParameters[]> vertices;
	size_t vertexCount;
	unique_ptr<unsigned int[]> indices;
	size_t indexCount;

	ModelData() : vertexCount(0), indexCount(0) {}

	ModelData(ModelData&& other) : 
		vertices(std::move(other.vertices)), vertexCount(other.vertexCount), 
		indices(std::move(other.indices)), indexCount(other.indexCount)
	{
	}

private:
	ModelData(const ModelData& other);
	ModelData& operator=(const ModelData& other);
};

template <typename T>
struct TypedDimensions
{
	T x, y, z;

	TypedDimensions(T x, T y, T z) : x(x), y(y), z(z) {}
};

#if !DEBUG
#define Assert(x)	(void)(x)
#elif WINDOWS_PHONE
#define Assert(x)   do \
					{ \
						if (!(x)) \
						{ \
							OutputDebugStringW(L"Assertion fail!\r\n"); \
							OutputDebugStringW((__WFILE__ + wstring(L": ") + to_wstring(__LINE__) + L"\r\n").c_str()); \
							__debugbreak(); \
						} \
					} \
					while (false)
#else
#define Assert(x)   do \
					{ \
						if (!(x)) \
						{ \
							OutputDebugStringW(L"Assertion fail!\r\n"); \
							OutputDebugStringW((__WFILE__ + wstring(L": ") + to_wstring(__LINE__) + L"\r\n").c_str()); \
							DebugBreak(); \
						} \
					} \
					while (false)
#endif

