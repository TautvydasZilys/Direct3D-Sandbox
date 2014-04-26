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

class IModel
{
protected:	
	IShader& m_Shader;
	
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	unsigned int m_IndexCount;

	static unordered_map<wstring, unique_ptr<const ModelData>> s_ModelDataCache;
	static unordered_map<ModelId, shared_ptr<IModel>, ModelIdHash> s_ModelCache;	
	static const IModel* s_ModelWhichLastSetParameters;

#if DEBUG
	wstring m_Key;
#endif

	IModel(IShader& shader
#if DEBUG
		, const wstring& modelPath
#endif
	);

	void InitializeIndexBuffer(const ModelData& modelData);

	static void InitializeModel(IShader& shader, const wstring& modelPath);
	static const ModelData& GetModelData(const wstring& key);
	static ComPtr<ID3D11Buffer> CreateIndexBuffer(unsigned int indexCount, unsigned int indices[]);

private:
	IModel(const IModel& other);														// Not implemented (no copying allowed)
	IModel& operator=(const IModel& other);												// Not implemented (no copying allowed)
	
public:
	IModel(IModel&& other);
	virtual ~IModel();

	static IModel& Get(const wstring& path, IShader& shader);
	static void InvalidateParameterSetter() { s_ModelWhichLastSetParameters = nullptr; }

	virtual void Render(const RenderParameters& renderParameters) = 0;
};

