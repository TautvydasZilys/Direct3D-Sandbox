#include "PrecompiledHeader.h"
#include "PixelShader.h"
#include "Tools.h"

PixelShader::PixelShader(ComPtr<ID3D11Device> device, wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = device->CreatePixelShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);

	Reflect();
}


PixelShader::~PixelShader()
{
}
