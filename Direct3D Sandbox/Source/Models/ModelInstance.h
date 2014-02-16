#pragma once

#include "Constants.h"
#include "Source\Graphics\Model.h"

struct ModelParameters
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 color;
};

class ModelInstance
{
private:
	Model& m_Model;
	DirectX::XMFLOAT4X4 m_WorldMatrix;
	DirectX::XMFLOAT4X4 m_InversedTransposedWorldMatrix;
	ComPtr<ID3D11ShaderResourceView> m_Texture;

protected:
	ModelParameters m_Parameters;

	void Initialize();

public:
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters);
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath);
	virtual ~ModelInstance();

	void SetPosition(const DirectX::XMFLOAT3& position);

	virtual void Render(RenderParameters& renderParameters);
};
