#include "PrecompiledHeader.h"
#include "ModelInstance.h"


ModelInstance::ModelInstance(ComPtr<ID3D11Device> device, IShader& shader, string modelPath, const ModelParameters& modelParameters) :
	m_Model(Model::Get(modelPath, shader))
{
}

ModelInstance::~ModelInstance()
{
}
