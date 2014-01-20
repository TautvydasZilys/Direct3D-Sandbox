#include "PrecompiledHeader.h"
#include "VertexShader.h"
#include "Tools.h"

VertexShader::VertexShader(ComPtr<ID3D11Device> device, wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = device->CreateVertexShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);

	Reflect(device, shaderBuffer);
}


VertexShader::~VertexShader()
{
}
