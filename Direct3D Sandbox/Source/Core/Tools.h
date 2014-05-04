#pragma once

#include "Parameters.h"
#include "PrecompiledHeader.h"

struct ModelData;

namespace Tools
{
	long long int GetRawTime();
	double GetTime();
	vector<uint8_t> ReadFileToVector(const wstring& path);
	unique_ptr<ModelData> LoadModel(const wstring& path);

	vector<wstring> GetFilesInDirectory(wstring path, const wstring& searchPattern, bool recursive);
	vector<wstring> GetDirectories(wstring path, bool recursive);
	bool DirectoryExists(const wstring& path);

	int GetMemoryUsage();

	string ToLower(const string& str);
	wstring ToLower(const wstring& str);

	void FatalError(const wstring& msg);

	namespace BufferReader
	{
		string ReadString(const vector<uint8_t>& buffer, unsigned int& position);
		unsigned int ReadUInt(const vector<uint8_t>& buffer, unsigned int& position);
		float ReadFloat(const vector<uint8_t>& buffer, unsigned int& position);
		char ReadChar(const vector<uint8_t>& buffer, unsigned int& position);
	}

	namespace Random
	{
		inline mt19937& GetRandomEngine() { static mt19937 engineInstance(static_cast<unsigned int>(Tools::GetRawTime())); return engineInstance; }

		template <typename T>
		inline T GetNextReal(T lowerBound, T higherBound) { return uniform_real_distribution<T>(lowerBound, higherBound)(GetRandomEngine()); }

		template <typename T>
		inline T GetNextInteger(T lowerBound, T higherBound) { return uniform_int_distribution<T>(lowerBound, higherBound)(GetRandomEngine()); }
	}
};

struct Point2D
{
	float x, y;

	Point2D() {}
	Point2D(float x, float y) : x(x), y(y) {}
};

enum ModelType
{
	Still = 0,
	Animated,
	ModelTypeCount
};

struct ModelData
{
	ModelType modelType;
	unique_ptr<VertexParameters[]> vertices;
	size_t vertexCount;
	unique_ptr<unsigned int[]> indices;
	size_t indexCount;

	float radius;

	ModelData() : vertexCount(0), indexCount(0), radius(0.0f) {}

	ModelData(ModelData&& other) : 
		vertices(std::move(other.vertices)), vertexCount(other.vertexCount), 
		indices(std::move(other.indices)), indexCount(other.indexCount),
		radius(other.radius)
	{
	}

private:
	ModelData(const ModelData& other);
	ModelData& operator=(const ModelData& other);
};

struct AnimatedModelState
{
	size_t frameCount;
	size_t frameOffset;

	AnimatedModelState() : frameCount(0), frameOffset(0) {}
};

struct AnimatedModelData : public ModelData
{
	size_t totalFrameCount;
	size_t stateCount;
	unique_ptr<AnimatedModelState[]> stateData;

	AnimatedModelData() : totalFrameCount(0), stateCount(0) {}

	AnimatedModelData(AnimatedModelData&& other) :
		ModelData(std::move(other)),
		totalFrameCount(other.totalFrameCount),
		stateCount(other.stateCount),
		stateData(std::move(other.stateData))
	{
	}

private:
	AnimatedModelData(const AnimatedModelData& other);
	AnimatedModelData& operator=(const AnimatedModelData& other);
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

