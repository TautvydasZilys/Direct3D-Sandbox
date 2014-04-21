#pragma once

#include "IModel.h"

class Model : 
	public IModel
{
private:
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	unsigned int m_VertexCount;
	unsigned int m_IndexCount;
	
	Model(IShader& shader, const wstring& modelPath);
	Model(IShader& shader, const ModelData& modelData);

	void CreateBuffers(const ModelData& modelData);
	
	Model(const Model& other);															// Not implemented (no copying allowed)
	Model& operator=(const Model& other);												// Not implemented (no copying allowed)

	template <typename _Ty1, typename _Ty2>
	friend struct pair;
	
	friend class std::_Ref_count_obj<Model>;

public:
	Model(Model&& other);
	virtual ~Model();
	
	static Model CreateNonCachedModel(const ModelData& modelData, IShader& shader);
	virtual void Render(const RenderParameters& renderParameters);
};