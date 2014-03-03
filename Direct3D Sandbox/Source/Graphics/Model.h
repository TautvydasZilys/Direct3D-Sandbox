#pragma once

#include "Tools.h"

class IShader;
struct RenderParameters;

struct ModelId
{
	const wstring path;
	const IShader& shader;

	ModelId(const wstring& path, const IShader& shader) : path(Tools::ToLower(path)), shader(shader) { }
	bool operator==(const ModelId& other) const { return path == other.path && (&shader == &other.shader); }
};

struct ModelIdHash
{
	size_t operator()(const ModelId& value) const
    {
		return hash<wstring>()(value.path) ^ hash<const void*>()(&value.shader);
    }
};

class Model
{
private:
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	unsigned int m_VertexCount;
	unsigned int m_IndexCount;
	IShader& m_Shader;
	
	static unordered_map<wstring, const ModelData> s_ModelDataCache;
	static unordered_map<ModelId, Model, ModelIdHash> s_ModelCache;	
	static const Model* s_ModelWhichLastSetParameters;
#if DEBUG
	wstring m_Key;
#endif
	
	Model(IShader& shader, const wstring& modelPath);
	Model(IShader& shader, const ModelData& modelData);

	void CreateBuffers(const ModelData& modelData);
	
	Model(const Model& other);															// Not implemented (no copying allowed)
	Model& operator=(const Model& other);												// Not implemented (no copying allowed)
	
	static void InitializeModel(IShader& shader, const wstring& modelPath);

	template <typename _Ty1, typename _Ty2>
	friend struct pair;

public:
	Model(Model&& other);
	~Model();

	static Model& Get(const wstring& path, IShader& shader);
	static Model CreateNonCachedModel(const ModelData& modelData, IShader& shader);
	
	void Render(const RenderParameters& renderParameters);
	static void InvalidateParameterSetter() { s_ModelWhichLastSetParameters = nullptr; }
};