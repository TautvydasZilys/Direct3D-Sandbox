#pragma once

#include "IModel.h"

class Model : 
	public IModel
{
private:
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	unsigned int m_VertexCount;
	
	Model(IShader& shader, const wstring& modelPath);
	Model(IShader& shader, const ModelData& modelData);

	void CreateBuffers(const ModelData& modelData);
	
	Model(const Model& other);															// Not implemented (no copying allowed)
	Model& operator=(const Model& other);												// Not implemented (no copying allowed)
		
	friend class std::_Ref_count_obj<Model>;

public:
	Model(Model&& other);
	virtual ~Model();
	
	static Model CreateNonCachedModel(const ModelData& modelData, IShader& shader);
	virtual void Render(const RenderParameters& renderParameters);
};