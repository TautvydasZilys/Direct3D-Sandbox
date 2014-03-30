#pragma once

#include "IModelInstance.h"
#include "Source\Graphics\Model.h"

struct ModelParameters
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 color;
};

class ModelInstance : public IModelInstance
{
private:
	Model& m_Model;
	DirectX::XMFLOAT4X4 m_WorldMatrix;
	DirectX::XMFLOAT4X4 m_InversedTransposedWorldMatrix;
	ComPtr<ID3D11ShaderResourceView> m_Texture;
	ComPtr<ID3D11ShaderResourceView> m_NormalMap;

	ModelInstance(const ModelInstance& other);

protected:
	ModelParameters m_Parameters;

	void Initialize();
	void SetPosition(const DirectX::XMFLOAT3& position);

public:
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters);
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath);
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath, const wstring& normalMapPath);
	virtual ~ModelInstance();

	virtual void UpdateAndRender(RenderParameters& renderParameters);
};

