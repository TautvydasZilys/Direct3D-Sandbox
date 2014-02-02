#include "PrecompiledHeader.h"
#include "ModelInstance.h"
#include "Parameters.h"
#include "Source\Graphics\Texture.h"

ModelInstance::ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters) :
	m_Model(Model::Get(modelPath, shader)),
	m_Color(modelParameters.color)
{
	Initialize(modelParameters);
}

ModelInstance::ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
								const wstring& texturePath) :
	m_Model(Model::Get(modelPath, shader)),
	m_Color(modelParameters.color),
	m_Texture(Texture::Get(texturePath))
{
	Initialize(modelParameters);
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::Initialize(const ModelParameters& modelParameters)
{
	DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(modelParameters.position.x, modelParameters.position.y, modelParameters.position.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(modelParameters.rotation.x, modelParameters.rotation.y, modelParameters.rotation.z);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(modelParameters.scale.x, modelParameters.scale.y, modelParameters.scale.z);

	DirectX::XMMATRIX worldMatrix = scale * rotation * position;
	
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixTranspose(worldMatrix));
	DirectX::XMStoreFloat4x4(&m_InversedTransposedWorldMatrix, DirectX::XMMatrixInverse(nullptr, worldMatrix));
}

void ModelInstance::Render(RenderParameters& renderParameters)
{
	memcpy(&renderParameters.worldMatrix, &m_WorldMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(&renderParameters.inversedTransposedWorldMatrix, &m_InversedTransposedWorldMatrix, sizeof(DirectX::XMFLOAT4X4));
	renderParameters.color = m_Color;
	renderParameters.texture = m_Texture.Get();

	m_Model.Render(renderParameters);
}
