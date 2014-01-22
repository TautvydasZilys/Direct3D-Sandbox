#include "PrecompiledHeader.h"
#include "ConstantBuffer.h"
#include "Tools.h"

ConstantBuffer::ConstantBuffer(ComPtr<ID3D11Device> device, ID3D11ShaderReflectionConstantBuffer* bufferReflection)
{
	HRESULT result;
	D3D11_BUFFER_DESC constantBufferDescription;
	D3D11_SHADER_BUFFER_DESC bufferDescription;
	D3D11_SHADER_VARIABLE_DESC fieldDescription;

	result = bufferReflection->GetDesc(&bufferDescription);
	Assert(result = S_OK);

#if DEBUG
	m_Name = bufferDescription.Name;
#endif

	for (auto i = 0u; i < bufferDescription.Variables; i++)
	{
		auto field = bufferReflection->GetVariableByIndex(i);

		result = field->GetDesc(&fieldDescription);
		Assert(result == S_OK);

		m_Fields.emplace_back(fieldDescription.Name, fieldDescription.StartOffset, fieldDescription.Size);
	}
	
	sort(begin(m_Fields), end(m_Fields));
	m_Size = bufferDescription.Size;

	constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDescription.ByteWidth = m_Size;
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDescription.MiscFlags = 0;
	constantBufferDescription.StructureByteStride = 0;

	result = device->CreateBuffer(&constantBufferDescription, nullptr, &m_Buffer);
	Assert(result == S_OK);
}

ConstantBuffer::ConstantBuffer(ConstantBuffer&& other) :
#if DEBUG
	m_Name(other.m_Name),
#endif
	m_Buffer(other.m_Buffer),
	m_Fields(std::move(other.m_Fields)),
	m_Size(other.m_Size)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	bool shouldCopyToGPU = false;

	for_each (begin(m_Fields), end(m_Fields), [&shouldCopyToGPU, &renderParameters](ConstantBufferField& field)
	{
		shouldCopyToGPU |= field.SetValueIfNeeded(reinterpret_cast<const uint8_t*>(&renderParameters) + field.GetParameterOffset());
	});

	if (shouldCopyToGPU)
	{
		SetRenderParametersNoCheck(deviceContext);
	}
}

void ConstantBuffer::SetRenderParametersNoCheck(ComPtr<ID3D11DeviceContext> deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	result = deviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Assert(result == S_OK);
	
	for_each (begin(m_Fields), end(m_Fields), [&mappedResource](ConstantBufferField& field)
	{
		memcpy(reinterpret_cast<uint8_t*>(mappedResource.pData) + field.GetByteOffset(), field.GetValue(), field.GetSize());
	});

	deviceContext->Unmap(m_Buffer.Get(), 0);
}