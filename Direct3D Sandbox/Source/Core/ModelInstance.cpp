#include "PrecompiledHeader.h"
#include "ModelInstance.h"
#include "Parameters.h"
#include "Source\Graphics\Texture.h"

ModelInstance::ModelInstance(ComPtr<ID3D11Device> device, IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters) :
	m_Model(Model::Get(modelPath, shader)),
	m_Parameters(modelParameters)
{
}

ModelInstance::ModelInstance(ComPtr<ID3D11Device> device, IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
								const wstring& texturePath) :
	m_Model(Model::Get(modelPath, shader)),
	m_Parameters(modelParameters),
	m_Texture(Texture::Get(texturePath))
{
}


ModelInstance::~ModelInstance()
{
}

void ModelInstance::Render(ComPtr<ID3D11DeviceContext> deviceContext, RenderParameters& renderParameters)
{
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_Parameters.scale.x, m_Parameters.scale.y, m_Parameters.scale.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(m_Parameters.rotation.x, m_Parameters.rotation.y, m_Parameters.rotation.z);
	DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(m_Parameters.position.x, m_Parameters.position.y, m_Parameters.position.z);

	DirectX::XMStoreFloat4x4(&renderParameters.worldMatrix, DirectX::XMMatrixTranspose(scale * rotation * position));
	renderParameters.color = m_Parameters.color;
	renderParameters.texture = m_Texture.Get();

	m_Model.Render(deviceContext, renderParameters);
}
