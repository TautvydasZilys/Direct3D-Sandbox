#include "PrecompiledHeader.h"
#include "ModelInstance.h"
#include "Parameters.h"
#include "Source\Graphics\Texture.h"

ModelInstance::ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters) :
	m_Model(IModel::Get(modelPath, shader)),
	m_Parameters(modelParameters)
{
}

ModelInstance::ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
								const wstring& texturePath) :
	m_Model(IModel::Get(modelPath, shader)),
	m_Parameters(modelParameters),
	m_Texture(Texture::Get(texturePath))
{
}

ModelInstance::ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
								const wstring& texturePath, const wstring& normalMapPath) :
	m_Model(IModel::Get(modelPath, shader)),
	m_Parameters(modelParameters),
	m_Texture(Texture::Get(texturePath)),
	m_NormalMap(Texture::Get(normalMapPath))
{
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::RecalculateWorldMatrix()
{
	DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(m_Parameters.position.x, m_Parameters.position.y, m_Parameters.position.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(m_Parameters.rotation.x, m_Parameters.rotation.y, m_Parameters.rotation.z);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_Parameters.scale.x, m_Parameters.scale.y, m_Parameters.scale.z);

	DirectX::XMMATRIX worldMatrix = scale * rotation * position;
	
	m_WorldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	m_InversedTransposedWorldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

const DirectX::XMMATRIX& ModelInstance::GetWorldMatrix()
{
	if (m_DirtyWorldMatrix)
	{
		RecalculateWorldMatrix();
		m_DirtyWorldMatrix = false;
	}
	
	return m_WorldMatrix;
}

const DirectX::XMMATRIX& ModelInstance::GetInversedTransposedWorldMatrix()
{
	if (m_DirtyWorldMatrix)
	{
		RecalculateWorldMatrix();
		m_DirtyWorldMatrix = false;
	}
	
	return m_InversedTransposedWorldMatrix;
}

void ModelInstance::SetPosition(const DirectX::XMFLOAT3& position)
{
	m_Parameters.position = position;
	m_DirtyWorldMatrix = true;
}

void ModelInstance::SetRotation(const DirectX::XMFLOAT3& rotation)
{
	m_Parameters.rotation = rotation;
	m_DirtyWorldMatrix = true;
}

void ModelInstance::UpdateAndRender(RenderParameters& renderParameters)
{
	auto& worldMatrix = GetWorldMatrix();

	memcpy(&renderParameters.worldMatrix, &worldMatrix, sizeof(DirectX::XMMATRIX));
	memcpy(&renderParameters.inversedTransposedWorldMatrix, &GetInversedTransposedWorldMatrix(), sizeof(DirectX::XMMATRIX));
	renderParameters.worldViewProjectionMatrix = renderParameters.viewProjectionMatrix * worldMatrix;

	renderParameters.color = m_Parameters.color;
	renderParameters.texture = m_Texture.Get();
	renderParameters.normalMap = m_NormalMap.Get();

	m_Model.Render(renderParameters);
}

void* ModelInstance::operator new(size_t size)
{
	void* ptr = _aligned_malloc(size, 16);

	if (ptr == nullptr)
	{
		throw bad_alloc();
	}

	return ptr;
}

void ModelInstance::operator delete(void* p)
{
	_aligned_free(p);
}