#pragma once
#include "ModelInstance.h"

class ModelInstance2D :
	public ModelInstance
{
public:
	ModelInstance2D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath);
	virtual ~ModelInstance2D();
	
	virtual void UpdateAndRender3D(RenderParameters& renderParameters) { }
	virtual void UpdateAndRender2D(RenderParameters& renderParameters);
};

