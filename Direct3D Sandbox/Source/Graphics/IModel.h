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

	static unordered_map<wstring, const ModelData> s_ModelDataCache;
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

	static void InitializeModel(IShader& shader, const wstring& modelPath);
	const ModelData& GetModelData(const wstring& key);

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

