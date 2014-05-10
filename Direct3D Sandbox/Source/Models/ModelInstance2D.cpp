#include "PrecompiledHeader.h"
#include "ModelInstance2D.h"


ModelInstance2D::ModelInstance2D(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath) :
	ModelInstance(shader, modelPath, modelParameters, texturePath)
{
}

ModelInstance2D::~ModelInstance2D()
{
}

void ModelInstance2D::UpdateAndRender2D(RenderParameters& renderParameters)
{
	SetRenderParameters(renderParameters);
	RenderModel(renderParameters);
}