#include "PrecompiledHeader.h"
#include "Source\Graphics\Model.h"
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

// edge1 = u1 * tangent + v1 * binormal
// edge2 = u2 * tangent + v2 * binormal
static void CalculateTangentsAndBinormals(ModelData& model)
{
	DirectX::XMVECTOR edges[2];
	DirectX::XMVECTOR tangent, binormal;
	float u1, u2, v1, v2;

	auto& vertices = model.vertices;
	auto& indices = model.indices;

	for (auto i = 0u; i < model.indexCount; i += 3)
	{
		edges[0] = DirectX::XMVectorSet(vertices[indices[i + 1]].position.x - vertices[indices[i]].position.x, 
										vertices[indices[i + 1]].position.y - vertices[indices[i]].position.y, 
										vertices[indices[i + 1]].position.z - vertices[indices[i]].position.z, 
										1.0f);

		edges[1] = DirectX::XMVectorSet(vertices[indices[i + 2]].position.x - vertices[indices[i]].position.x, 
										vertices[indices[i + 2]].position.y - vertices[indices[i]].position.y, 
										vertices[indices[i + 2]].position.z - vertices[indices[i]].position.z, 
										1.0f);
		
		u1 = vertices[indices[i + 1]].textureCoordinates.x - vertices[indices[i]].textureCoordinates.x;
		u2 = vertices[indices[i + 2]].textureCoordinates.x - vertices[indices[i]].textureCoordinates.x;

		v1 = vertices[indices[i + 1]].textureCoordinates.y - vertices[indices[i]].textureCoordinates.y;
		v2 = vertices[indices[i + 2]].textureCoordinates.y - vertices[indices[i]].textureCoordinates.y;
		
		tangent = DirectX::XMVectorScale(DirectX::XMVectorSubtract(DirectX::XMVectorScale(edges[1], v1), DirectX::XMVectorScale(edges[0], v2)), 1.0f / (u2 * v1 - u1 * v2));
		binormal = DirectX::XMVectorScale(DirectX::XMVectorSubtract(DirectX::XMVectorScale(edges[1], u1), DirectX::XMVectorScale(edges[0], u2)), 1.0f / (u1 * v2 - u2 * v1));
		
		DirectX::XMStoreFloat3(&vertices[indices[i]].tangent, tangent);
		DirectX::XMStoreFloat3(&vertices[indices[i]].binormal, binormal);
		DirectX::XMStoreFloat3(&vertices[indices[i + 1]].tangent, tangent);
		DirectX::XMStoreFloat3(&vertices[indices[i + 1]].binormal, binormal);
		DirectX::XMStoreFloat3(&vertices[indices[i + 2]].tangent, tangent);
		DirectX::XMStoreFloat3(&vertices[indices[i + 2]].binormal, binormal);
	}
}

static void OptimizeModel(ModelData& model)
{
	auto vertexPtrHash = [](const VertexParameters* value) -> size_t
	{
		return hash<VertexParameters>()(*value);
	};

	auto compareVertexPtrValues = [](const VertexParameters* first, const VertexParameters* second) -> bool
	{
		return *first == *second;
	};

	unordered_map<const VertexParameters*, int, size_t(*)(const VertexParameters*), 
		bool(*)(const VertexParameters*, const VertexParameters*)> vertexMap(16, vertexPtrHash, compareVertexPtrValues);

	map<int, int> indexChanges;

	for (auto i = 0u; i < model.vertexCount; i++)
	{
		vertexMap.insert(make_pair(&model.vertices[i], i));
	}

	int i = 0;
	unique_ptr<VertexParameters[]> optimizedVertices(new VertexParameters[vertexMap.size()]);

	for (auto& vertex : vertexMap)
	{
		memcpy(&optimizedVertices[i], &vertex.first, sizeof(VertexParameters));
		indexChanges[vertex.second] = i;

		i++;
	}

	for (auto i = 0u; i < model.indexCount; i++)
	{
		model.indices[i] = indexChanges[vertexMap[&model.vertices[model.indices[i]]]];
	}

	model.vertices = std::move(optimizedVertices);
}

static ModelData ParseFaces(const vector<DirectX::XMFLOAT4>& coordinates, const vector<DirectX::XMFLOAT2>& textures,
							const vector<DirectX::XMFLOAT3>& normals, vector<string>& faces, bool shouldInverse)
{
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);
	auto const facesCount = faces.size();
	ModelData model;

	model.indexCount = model.vertexCount = faces.size();
	model.vertices = unique_ptr<VertexParameters[]>(new VertexParameters[model.vertexCount]);
	model.indices = unique_ptr<int[]>(new int[model.indexCount]);

	for (int i = 0; i < facesCount;)
	{
		for (int j = 0; j < 3; j++, i++)
		{
			char* cStr = &faces[i][0];
			long int v, t, n;

			v = strtol(cStr, &cStr, 10);
			cStr++;
			
			auto hasTexture = *cStr != '/';
			if (hasTexture)
			{
				t = strtol(cStr, &cStr, 10);
			}
			cStr++;

			n = strtol(cStr, nullptr, 10);

			model.vertices[i].position = coordinates[v - 1];
			model.vertices[i].normal = normals[n - 1];
			model.vertices[i].color = color;

			if (hasTexture)
			{
				model.vertices[i].textureCoordinates = textures[t - 1];
			}
			else
			{
				model.vertices[i].textureCoordinates = DirectX::XMFLOAT2(0.0f, 0.0f);
			} 
		}
				
		if (shouldInverse)
		{
			model.indices[i - 3] = i - 3;
			model.indices[i - 2] = i - 2;
			model.indices[i - 1] = i - 1;
		}
		else
		{
			model.indices[i - 3] = i - 3;
			model.indices[i - 2] = i - 1;
			model.indices[i - 1] = i - 2;
		}
	}

	return model;
}

ModelData Tools::LoadModel(const string& path, bool shouldInverse)
{
	vector<DirectX::XMFLOAT4> coordinates;
	vector<DirectX::XMFLOAT2> textures;
	vector<DirectX::XMFLOAT3> normals;
	vector<string> faces;
	
	float x, y, z;
	string input;

	ifstream in(path);
	Assert(in.is_open());

	while (!in.eof())
	{
		in >> input;

		if (!in.fail())
		{
			if (input == "v")
			{
				in >> x >> y >> z;
				coordinates.emplace_back(x, y, z, 1.0f);
			}
			else if (input == "vt")
			{
				in >> x >> y;
				textures.emplace_back(x, 1.0f - y);
			}
			else if (input == "vn")
			{
				in >> x >> y >> z;
				normals.emplace_back(x, y, z);
			}
			else if (input == "f")
			{
				for (int i = 0; i < 3; i++)
				{
					in >> input;
					faces.push_back(input);
				}
			}
		}
	}
	
	in.close();

	auto model = ParseFaces(coordinates, textures, normals, faces, shouldInverse);

	OptimizeModel(model);
	CalculateTangentsAndBinormals(model);

	return model;
}