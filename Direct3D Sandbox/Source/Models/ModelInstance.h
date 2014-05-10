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

	ModelParameters() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), color(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}
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

	void RecalculateWorldMatrix();

	ModelInstance(const ModelInstance& other);

protected:
	ModelParameters m_Parameters;

	const DirectX::XMMATRIX& GetWorldMatrix();
	const DirectX::XMMATRIX& GetInversedTransposedWorldMatrix();
	float GetModelRadius() const { return m_Model.GetRadius(); }
	
	virtual void SetRenderParameters(RenderParameters& renderParameters);
	inline void RenderModel(RenderParameters& renderParameters) { m_Model.Render(renderParameters); }

	void DirtyWorldMatrix() { m_DirtyWorldMatrix = true; }
	
public:
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters);
	ModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath);
	virtual ~ModelInstance();

	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetRotation(const DirectX::XMFLOAT3& rotation);
	void SetScale(const DirectX::XMFLOAT3& scale);

	float HorizontalDistanceSqrTo(const DirectX::XMFLOAT2& position);
};

