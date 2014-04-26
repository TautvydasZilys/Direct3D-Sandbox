#pragma once

#include "Parameters.h"
#include "PrecompiledHeader.h"

struct ModelData;

namespace Tools
{
	long long int GetRawTime();
	double GetTime();
	vector<uint8_t> ReadFileToVector(const wstring& path);
	ModelData LoadModel(const wstring& path, bool shouldInvert);

	vector<wstring> GetFilesInDirectory(wstring path, const wstring& searchPattern, bool recursive);	
	bool DirectoryExists(const wstring& path);

	inline mt19937& GetRandomEngine() { static mt19937 engineInstance(static_cast<unsigned int>(Tools::GetRawTime())); return engineInstance; }
	int GetMemoryUsage();

	string ToLower(const string& str);
	wstring ToLower(const wstring& str);

	namespace BufferReader
	{
		string ReadString(const vector<uint8_t>& buffer, unsigned int& position);
		unsigned int ReadUInt(const vector<uint8_t>& buffer, unsigned int& position);
		float ReadFloat(const vector<uint8_t>& buffer, unsigned int& position);
		char ReadChar(const vector<uint8_t>& buffer, unsigned int& position);
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

struct AnimatedModelData : public ModelData
{
	size_t frameCount;

	AnimatedModelData() : frameCount(0) {}

	AnimatedModelData(AnimatedModelData&& other) :
		ModelData(std::move(other)),
		frameCount(other.frameCount)
	{
	}

private:
	AnimatedModelData(const AnimatedModelData& other);
	AnimatedModelData& operator=(const AnimatedModelData& other);
};

enum ModelType
{
	StillModel = 0,
	AnimatedModel,
	ModelTypeCount
};

template <typename T>
struct TypedDimensions
{
	T x, y, z;

	TypedDimensions(T x, T y, T z) : x(x), y(y), z(z) {}
};

#if !DEBUG
#define Assert(x)
#else
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
#endif

