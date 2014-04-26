#include "PrecompiledHeader.h"
#include "..\..\Source\Core\Tools.h"
#include "ModelProcessor.h"

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
	cout << "Optimizing model..." << endl;
	cout << "\tVertex count before: " << model.vertexCount << endl;
	cout << "\tVertex count after:  " << vertexMap.size() << endl << endl;

	// Preserve original order by sorting by index
	vector<pair<const VertexParameters*, int>> vertexVector(begin(vertexMap), end(vertexMap));
	sort(begin(vertexVector), end(vertexVector), [](const pair<const VertexParameters*, int>& left, const pair<const VertexParameters*, int>& right) -> bool
	{
		return left.second < right.second;
	});

	unique_ptr<VertexParameters[]> optimizedVertices(new VertexParameters[vertexMap.size()]);

	for (auto& vertex : vertexVector)
	{
		memcpy(&optimizedVertices[i], &*vertex.first, sizeof(VertexParameters));
		indexChanges[vertex.second] = i;

		i++;
	}

	for (auto i = 0u; i < model.indexCount; i++)
	{
		model.indices[i] = indexChanges[vertexMap[&model.vertices[model.indices[i]]]];
	}

	model.vertices = std::move(optimizedVertices);
	model.vertexCount = vertexMap.size();
}

static ModelData ParseFaces(const vector<DirectX::XMFLOAT4>& coordinates, const vector<DirectX::XMFLOAT2>& textures,
							const vector<DirectX::XMFLOAT3>& normals, vector<string>& faces)
{
	auto const facesCount = faces.size();
	ModelData model;

	model.indexCount = model.vertexCount = faces.size();
	model.vertices = unique_ptr<VertexParameters[]>(new VertexParameters[model.vertexCount]);
	model.indices = unique_ptr<unsigned int[]>(new unsigned int[model.indexCount]);

	for (auto i = 0u; i < facesCount;)
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

			if (hasTexture)
			{
				model.vertices[i].textureCoordinates = textures[t - 1];
			}
			else
			{
				model.vertices[i].textureCoordinates = DirectX::XMFLOAT2(0.0f, 0.0f);
			}
			
			model.vertices[i].binormal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			model.vertices[i].tangent = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		}

		model.indices[i - 3] = i - 3;
		model.indices[i - 2] = i - 1;
		model.indices[i - 1] = i - 2;
	}

	return model;
}

static ModelData LoadModel(const wstring& path)
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

	auto model = ParseFaces(coordinates, textures, normals, faces);

	OptimizeModel(model);
	CalculateTangentsAndBinormals(model);

	return model;
}

static void SaveModel(const wstring& path, const ModelData& model)
{	
	ofstream out(path, ios::binary);

	// Model type
	auto modelType = ModelType::StillModel;
	out.write(reinterpret_cast<const char*>(&modelType), sizeof(ModelType));

	// Vertices
	out.write(reinterpret_cast<const char*>(&model.vertexCount), sizeof(int));
	out.write(reinterpret_cast<const char*>(model.vertices.get()), model.vertexCount * sizeof(VertexParameters));

	// Indices
	out.write(reinterpret_cast<const char*>(&model.indexCount), sizeof(int));
	out.write(reinterpret_cast<const char*>(model.indices.get()), model.indexCount * sizeof(unsigned int));

	out.close();
}

static void SaveAnimatedModel(const wstring& path, const AnimatedModelData& model)
{
	ofstream out(path, ios::binary);
	
	// Model type
	auto modelType = ModelType::AnimatedModel;
	out.write(reinterpret_cast<const char*>(&modelType), sizeof(ModelType));
	out.write(reinterpret_cast<const char*>(&model.frameCount), sizeof(int));

	// Vertices
	out.write(reinterpret_cast<const char*>(&model.vertexCount), sizeof(int));
	out.write(reinterpret_cast<const char*>(model.vertices.get()), model.frameCount * model.vertexCount * sizeof(VertexParameters));

	// Indices
	out.write(reinterpret_cast<const char*>(&model.indexCount), sizeof(int));
	out.write(reinterpret_cast<const char*>(model.indices.get()), model.indexCount * sizeof(unsigned int));

	out.close();
}

void ModelProcessor::ProcessModel(const wstring& path, const wstring& outputPath)
{
	auto modelName = path.substr(path.find_last_of(L'\\') + 1);		// Remove folder
	modelName = modelName.substr(0, modelName.length() - 4);		// Remove extension

	if (outputPath[outputPath.length() - 1] != L'\\')
	{
		modelName = outputPath +  L'\\' + modelName;
	}
	else
	{
		modelName = outputPath + modelName;
	}
	
	auto model = LoadModel(path);

	SaveModel(modelName + L".model", model);
}

void ModelProcessor::ProcessAnimatedModel(const wstring& rootPath, const wstring& outputPath)
{
	auto frames = Tools::GetFilesInDirectory(rootPath, L"*.obj", false);
	if (frames.size() == 0)
	{
		return;
	}

	sort(begin(frames), end(frames));

	vector<ModelData> modelFrames;

	for (auto i = 0u; i < frames.size(); i++)
	{
		wcout << "Loading frame " << i << "..." << endl;
		modelFrames.push_back(LoadModel(frames[i]));
	}

	AnimatedModelData animatedModelData;
	animatedModelData.frameCount = frames.size() - 1;
	animatedModelData.indexCount = modelFrames[0].indexCount;
	animatedModelData.vertexCount = modelFrames[0].vertexCount;

	bool allFramesMatch = true;
	vector<string> errors;

	// Check whether all models actually match
	for (auto i = 1u; i < animatedModelData.frameCount; i++)
	{
		// Validate vertices
		if (modelFrames[i].vertexCount != modelFrames[0].vertexCount)
		{
			allFramesMatch = false;
			errors.push_back("ERROR: Frame " + to_string(i) + " index count doesn't match frame 0 index count!\r\n");
		}

		// Validate indices
		if (modelFrames[i].indexCount != modelFrames[0].indexCount)
		{
			allFramesMatch = false;
			errors.push_back("ERROR: Frame " + to_string(i) + " index count doesn't match frame 0 index count!\r\n");
		}
		else
		{
			for (auto j = 0u; j < animatedModelData.indexCount; j++)
			{
				if (modelFrames[i].indices[j] != modelFrames[0].indices[j])
				{
					allFramesMatch = false;
					errors.push_back("ERROR: Frame " + to_string(i) + " index " + to_string(j) + 
						" doesn't match frame 0 index " + to_string(j) + "!\r\n");
				}
			}
		}
	}

	if (!allFramesMatch)
	{
		int errorCount = static_cast<int>(min(1000, errors.size()));
		for (int i = 0; i < errorCount; i++)
		{
			cout << errors[i];
		}
		exit(1);
	}

	animatedModelData.indices = std::move(modelFrames[0].indices);
	animatedModelData.vertices = unique_ptr<VertexParameters[]>(new VertexParameters[modelFrames[0].vertexCount * animatedModelData.frameCount]);

	for (auto i = 0u; i < animatedModelData.frameCount; i++)
	{
		auto vertexOffset = animatedModelData.vertexCount * i;

		for (auto j = 0u; j < animatedModelData.vertexCount; j++)
		{
			auto& targetVertex = animatedModelData.vertices[vertexOffset + j];
			
			targetVertex.position = modelFrames[i].vertices[j].position;
			targetVertex.textureCoordinates = modelFrames[i].vertices[j].textureCoordinates;
			targetVertex.normal = modelFrames[i].vertices[j].normal;
			targetVertex.tangent = modelFrames[i].vertices[j].tangent;
			targetVertex.binormal = modelFrames[i].vertices[j].binormal;

			targetVertex.position2 = modelFrames[i + 1].vertices[j].position;
			targetVertex.normal = modelFrames[i + 1].vertices[j].normal;
			targetVertex.tangent = modelFrames[i + 1].vertices[j].tangent;
			targetVertex.binormal = modelFrames[i + 1].vertices[j].binormal;
		}
	}

	auto modelName = rootPath.substr(rootPath.find_last_of('\\') + 1) + L".animatedModel";
	auto modelPath = outputPath + L"\\" + modelName;
	wcout << L"Saving animated model to \"" << modelPath << "\"...";
	SaveAnimatedModel(modelPath, animatedModelData);
	wcout << " Done!" << endl << endl;
}