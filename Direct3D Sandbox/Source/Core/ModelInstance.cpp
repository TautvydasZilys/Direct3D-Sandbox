#include "PrecompiledHeader.h"
#include "ModelInstance.h"
#include "Parameters.h"
#include "Source\Graphics\Texture.h"

ModelInstance::ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters) :
	m_Model(Model::Get(modelPath, shader)),
	m_Parameters(modelParameters)
{
	Initialize();
}

ModelInstance::ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
								const wstring& texturePath) :
	m_Model(Model::Get(modelPath, shader)),
	m_Parameters(modelParameters),
	m_Texture(Texture::Get(texturePath))
{
	Initialize();
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::Initialize()
{
	DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(m_Parameters.position.x, m_Parameters.position.y, m_Parameters.position.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(m_Parameters.rotation.x, m_Parameters.rotation.y, m_Parameters.rotation.z);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_Parameters.scale.x, m_Parameters.scale.y, m_Parameters.scale.z);

	DirectX::XMMATRIX worldMatrix = scale * rotation * position;
	
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixTranspose(worldMatrix));
	DirectX::XMStoreFloat4x4(&m_InversedTransposedWorldMatrix, DirectX::XMMatrixInverse(nullptr, worldMatrix));
}

void ModelInstance::SetPosition(const DirectX::XMFLOAT3& position)
{
	m_Parameters.position = position;
	Initialize();
}

void ModelInstance::Render(RenderParameters& renderParameters)
{
	memcpy(&renderParameters.worldMatrix, &m_WorldMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(&renderParameters.inversedTransposedWorldMatrix, &m_InversedTransposedWorldMatrix, sizeof(DirectX::XMFLOAT4X4));
	renderParameters.color = m_Parameters.color;
	renderParameters.texture = m_Texture.Get();

	m_Model.Render(renderParameters);
}
