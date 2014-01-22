#pragma once

class IShader;
struct RenderParameters;

struct ModelId
{
	string path;
	IShader& shader;

	ModelId(string path, IShader& shader) : path(path), shader(shader) { }
	bool operator==(const ModelId& other) { return path == other.path && (&shader == &other.shader); }
};

struct ModelIdHash
{
	size_t operator()(const ModelId& value) const
    {
		return hash<string>()(value.path) ^ hash<void*>()(&value.shader);
    }
};

class Model
{
private:
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	ComPtr<ID3D11Buffer> m_IndexBuffer;

	int m_IndexCount;

	static unordered_map<ModelId, Model, ModelIdHash> s_ModelCache;
	string m_Key;
	
	Model(ComPtr<ID3D11Device> device, IShader& shader, string modelPath);

public:
	~Model();

	void InitializeModel(ComPtr<ID3D11Device> device, IShader& shader, string modelPath);
	void Render(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);

	static Model& Get(string path);
};