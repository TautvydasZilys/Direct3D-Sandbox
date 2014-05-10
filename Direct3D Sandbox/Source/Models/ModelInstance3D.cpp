#include "PrecompiledHeader.h"

#include "ModelInstance3D.h"
#include "Source\Graphics\Texture.h"


ModelInstance3D::ModelInstance3D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters) :
	ModelInstance(shader, modelPath, modelParameters)
{
}

ModelInstance3D::ModelInstance3D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
								const wstring& texturePath) :
	ModelInstance(shader, modelPath, modelParameters, texturePath)
{
}

ModelInstance3D::ModelInstance3D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
								const wstring& texturePath, const wstring& normalMapPath) :
	ModelInstance(shader, modelPath, modelParameters, texturePath),
	m_NormalMap(Texture::Get(normalMapPath))
{
}

ModelInstance3D::~ModelInstance3D()
{
}

bool ModelInstance3D::IsInCameraFrustum(const RenderParameters& renderParameters) const
{
	auto radius = GetModelRadius() * max(max(m_Parameters.scale.x, m_Parameters.scale.y), m_Parameters.scale.z);
	auto radiusSqr = radius * radius;
	auto positionVector = DirectX::XMVectorSet(m_Parameters.position.x, m_Parameters.position.y, m_Parameters.position.z, 1.0f);

	for (int i = 0; i < 6; i++)
	{
		auto dotResult = DirectX::XMVector4Dot(renderParameters.frustumPlanes[i], positionVector);

		if (DirectX::XMVectorGetX(dotResult) + radius < 0.0f)
		{
			return false;
		}
	}

	return true;
}

void ModelInstance3D::SetRenderParameters(RenderParameters& renderParameters)
{
	memcpy(&renderParameters.inversedTransposedWorldMatrix, &GetInversedTransposedWorldMatrix(), sizeof(DirectX::XMMATRIX));
	renderParameters.normalMap = m_NormalMap.Get();
	ModelInstance::SetRenderParameters(renderParameters);
}

void ModelInstance3D::UpdateAndRender3D(RenderParameters& renderParameters)
{
#if ENABLE_FRUSTUM_CULLING
	if (!IsInCameraFrustum(renderParameters))
	{
		return;
	}
#endif

	SetRenderParameters(renderParameters);
	RenderModel(renderParameters);
}