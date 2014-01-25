#pragma once

#include "Source\Graphics\Model.h"

struct ModelParameters
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
};

class ModelInstance
{
private:
	Model& m_Model;
	ModelParameters m_Parameters;

public:
	ModelInstance(ComPtr<ID3D11Device> device, IShader& shader, wstring modelPath, const ModelParameters& modelParameters);
	~ModelInstance();

	void Render(ComPtr<ID3D11DeviceContext> deviceContext, RenderParameters& renderParameters);
};

