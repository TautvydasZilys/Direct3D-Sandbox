#pragma once

#include "AlignedClass.h"
#include "IModelInstance.h"
#include "Source\Graphics\Model.h"

struct ModelParameters
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 color;
};

class ModelInstance : 
	public IModelInstance, public AlignedClass
{
private:
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_InversedTransposedWorldMatrix;
	bool m_DirtyWorldMatrix;

	IModel& m_Model;
	ComPtr<ID3D11ShaderResourceView> m_Texture;
	ComPtr<ID3D11ShaderResourceView> m_NormalMap;

	bool IsInCameraFrustum(const RenderParameters& renderParameters) const;
	void RecalculateWorldMatrix();
	const DirectX::XMMATRIX& GetWorldMatrix();
	const DirectX::XMMATRIX& GetInversedTransposedWorldMatrix();

	ModelInstance(const ModelInstance& other);

protected:
	ModelParameters m_Parameters;

	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetRotation(const DirectX::XMFLOAT3& rotation);
	void DirtyWorldMatrix() { m_DirtyWorldMatrix = true; }

public:
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters);
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath);
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath, const wstring& normalMapPath);
	virtual ~ModelInstance();
	
	virtual void UpdateAndRender(RenderParameters& renderParameters);
	virtual void UpdateAndRender2D(RenderParameters& renderParameters) { }
};

