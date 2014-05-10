#pragma once

#include "ModelInstance.h"

class ModelInstance3D :
	public ModelInstance
{
private:
	ComPtr<ID3D11ShaderResourceView> m_NormalMap;
	
	bool IsInCameraFrustum(const RenderParameters& renderParameters) const;	
	virtual void SetRenderParameters(RenderParameters& renderParameters);

public:
	ModelInstance3D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters);
	ModelInstance3D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath);
	ModelInstance3D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath, const wstring& normalMapPath);
	virtual ~ModelInstance3D();

	virtual void UpdateAndRender3D(RenderParameters& renderParameters);
	virtual void UpdateAndRender2D(RenderParameters& renderParameters) { }
};

