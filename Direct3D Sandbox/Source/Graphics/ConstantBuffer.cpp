#include "PrecompiledHeader.h"
#include "ConstantBuffer.h"
#include "Tools.h"

ConstantBuffer::ConstantBuffer(ComPtr<ID3D11ShaderReflectionConstantBuffer> bufferReflection)
{
	HRESULT result;
	D3D11_SHADER_BUFFER_DESC bufferDescription;
	ComPtr<ID3D11ShaderReflectionVariable> field;

	result = bufferReflection->GetDesc(&bufferDescription);
	Assert(result);

	for (auto i = 0u; i < bufferDescription.Variables; i++)
	{
		field = bufferReflection->GetVariableByIndex(i);

	}
}

ConstantBuffer::~ConstantBuffer()
{
}
