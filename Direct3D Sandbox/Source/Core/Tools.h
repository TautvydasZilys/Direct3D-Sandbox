#pragma once

#include "PrecompiledHeader.h"
#include "Source\Graphics\Parameters.h"

struct ModelData;

namespace Tools
{
	float GetTime();
	vector<uint8_t> ReadFileToVector(const wstring& path);
	ModelData LoadModel(const string& path, bool shouldInverse);
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
	unique_ptr<int[]> indices;
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


#if !DEBUG
#define Assert(x)	(void)(x)
#elif WINDOWS_PHONE
#define Assert(x)   do \
					{ \
						if (!(x)) \
						{ \
							OutputDebugStringW(L"Fatal Error!\r\n"); \
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
							OutputDebugStringW(L"Fatal Error!\r\n"); \
							OutputDebugStringW((__WFILE__ + wstring(L": ") + to_wstring(__LINE__)).c_str()); \
							DebugBreak(); \
						} \
					} \
					while (false)
#endif

