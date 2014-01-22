#include "PrecompiledHeader.h"
#include "AutoShader.h"
#include "Tools.h"

AutoShader::AutoShader(ComPtr<ID3D11Device> device, wstring vertexShaderPath, wstring pixelShaderPath) :
	vertexShader(device, vertexShaderPath), pixelShader(device, pixelShaderPath)
{
}

AutoShader::~AutoShader()
{
}

void AutoShader::Render(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{

}