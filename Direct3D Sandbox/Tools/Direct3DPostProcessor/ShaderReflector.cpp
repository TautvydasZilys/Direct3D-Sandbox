#include "PrecompiledHeader.h"
#include "ShaderReflector.h"
#include "Tools.h"

static void AddUInt(vector<uint8_t>& buffer, unsigned int& byteOffset, unsigned int value)
{
	memcpy(&buffer[byteOffset], &value, 4);
	byteOffset += 4;
}

static void AddString(vector<uint8_t>& buffer, unsigned int& byteOffset, const string& str)
{
	auto length = str.length() + 1;		// Length + null terminator
	memcpy(&buffer[byteOffset], str.c_str(), length);
	byteOffset += length;
}

static void GetDXGIFormatAndSize(int mask, D3D_REGISTER_COMPONENT_TYPE componentType, DXGI_FORMAT& dxgiFormat, unsigned int& size)
{
	if (mask == 1)
	{
		switch (componentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			dxgiFormat = DXGI_FORMAT_R32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			dxgiFormat = DXGI_FORMAT_R32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			dxgiFormat = DXGI_FORMAT_R32_FLOAT;
			break;
		}

		size = 4;
	}
	else if (mask <= 3)
	{
		switch (componentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			dxgiFormat = DXGI_FORMAT_R32G32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			dxgiFormat = DXGI_FORMAT_R32G32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			break;
		}

		size = 8;
	}
	else if (mask <= 7)
	{
		switch (componentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			dxgiFormat = DXGI_FORMAT_R32G32B32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			dxgiFormat = DXGI_FORMAT_R32G32B32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		}

		size = 12;
	}
	else
	{
		switch (componentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			dxgiFormat = DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			dxgiFormat = DXGI_FORMAT_R32G32B32A32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}

		size = 16;
	}
}

static void ReflectOnConstantBuffer(vector<uint8_t>& metadataBuffer, ID3D11ShaderReflectionConstantBuffer* bufferReflection)
{	
	HRESULT result;
	D3D11_SHADER_BUFFER_DESC bufferDescription;
	D3D11_SHADER_VARIABLE_DESC fieldDescription;
	unsigned int parameterOffset;
	
	result = bufferReflection->GetDesc(&bufferDescription);
	Assert(result == S_OK);

	auto byteOffset = metadataBuffer.size();
	metadataBuffer.resize(byteOffset + 4 + 12 * bufferDescription.Variables);
	AddUInt(metadataBuffer, byteOffset, bufferDescription.Variables);

	for (auto i = 0u; i < bufferDescription.Variables; i++)
	{
		auto field = bufferReflection->GetVariableByIndex(i);

		result = field->GetDesc(&fieldDescription);
		Assert(result == S_OK);
		
		parameterOffset = RenderParameters::GetFieldByteOffset(fieldDescription.Name);
		Assert(parameterOffset != 0xFFFFFFFF);
		
		AddUInt(metadataBuffer, byteOffset, parameterOffset);
		AddUInt(metadataBuffer, byteOffset, fieldDescription.StartOffset);
		AddUInt(metadataBuffer, byteOffset, fieldDescription.Size);
	}
}

static void ReflectOnInputLayout(vector<uint8_t>& metadataBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
									const D3D11_SHADER_DESC& shaderDescription)
{	
	HRESULT result;
	D3D11_SIGNATURE_PARAMETER_DESC parameterDescription;
	DXGI_FORMAT dxgiFormat;
	unsigned int itemSize, parameterOffset;
	string semanticName;

	auto byteOffset = metadataBuffer.size();
	metadataBuffer.resize(byteOffset + 4);
	AddUInt(metadataBuffer, byteOffset, shaderDescription.InputParameters);

	for (auto i = 0u; i < shaderDescription.InputParameters; i++)
	{
		result = shaderReflection->GetInputParameterDesc(i, &parameterDescription);
		Assert(result == S_OK);

		GetDXGIFormatAndSize(parameterDescription.Mask, parameterDescription.ComponentType, dxgiFormat, itemSize);
		semanticName = parameterDescription.SemanticName;
		parameterOffset = VertexParameters::GetFieldByteOffset(semanticName);

		metadataBuffer.resize(metadataBuffer.size() + 17 + semanticName.length());

		AddString(metadataBuffer, byteOffset, semanticName);
		AddUInt(metadataBuffer, byteOffset, parameterDescription.SemanticIndex);
		AddUInt(metadataBuffer, byteOffset, dxgiFormat);
		AddUInt(metadataBuffer, byteOffset, itemSize);
		AddUInt(metadataBuffer, byteOffset, parameterOffset);
	}
}

static void AddResource(vector<uint8_t>& metadataBuffer, unsigned int type, const string& name)
{
	auto byteOffset = metadataBuffer.size();

	metadataBuffer.resize(byteOffset + 5 + name.length());
	AddUInt(metadataBuffer, byteOffset, type);
	AddString(metadataBuffer, byteOffset, name);
}


static void ReflectOnOtherResources(vector<uint8_t>& metadataBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
										const D3D11_SHADER_DESC& shaderDescription)
{	
	HRESULT result;
	D3D11_SHADER_INPUT_BIND_DESC desc;
	unsigned int numberOfResources = 0;

	auto positionForResourceCount = metadataBuffer.size();
	metadataBuffer.resize(positionForResourceCount + 4);

	for (auto i = 0u; i < shaderDescription.BoundResources; i++)
	{
		result = shaderReflection->GetResourceBindingDesc(i, &desc);
		Assert(result == S_OK);

		switch (desc.Type)
		{
		case D3D_SIT_SAMPLER:
		case D3D_SIT_TEXTURE:
			AddResource(metadataBuffer, desc.Type, desc.Name);
			numberOfResources++;
			break;
		}
	}

	AddUInt(metadataBuffer, positionForResourceCount, numberOfResources);
}

vector<uint8_t> ReflectShaderImpl(const vector<uint8_t>& shaderBuffer)
{
	HRESULT result;
	ComPtr<ID3D11ShaderReflection> shaderReflection;
	D3D11_SHADER_DESC shaderDescription;
	unsigned int byteOffset = 12;
	unsigned int segmentPositionOffset;
	vector<uint8_t> metadataBuffer(16);

	result = D3DReflect(shaderBuffer.data(), shaderBuffer.size(), IID_ID3D11ShaderReflection, &shaderReflection);
	Assert(result == S_OK);
		
	result = shaderReflection->GetDesc(&shaderDescription);
	Assert(result == S_OK);

	segmentPositionOffset = 0;
	AddUInt(metadataBuffer, segmentPositionOffset, byteOffset);
	AddUInt(metadataBuffer, byteOffset, shaderDescription.ConstantBuffers);
	for (auto i = 0u; i < shaderDescription.ConstantBuffers; i++)
	{
		auto bufferReflection = shaderReflection->GetConstantBufferByIndex(i);
		ReflectOnConstantBuffer(metadataBuffer, bufferReflection);
	}
	
	segmentPositionOffset = 4;
	AddUInt(metadataBuffer, segmentPositionOffset, metadataBuffer.size());
	ReflectOnInputLayout(metadataBuffer, shaderReflection, shaderDescription);

	segmentPositionOffset = 8;
	AddUInt(metadataBuffer, segmentPositionOffset, metadataBuffer.size());
	ReflectOnOtherResources(metadataBuffer, shaderReflection, shaderDescription);

	return metadataBuffer;
}

// Metadata format:
//
// 12 bytes - metadata file index of byte offsets
//		4 bytes - offset of constant buffer data
//		4 bytes - offset of input layout data
//		4 bytes - offset of texture data
//
// 4 bytes - number of constant buffers
// n Constants buffer data:
//		4 bytes - number of constant buffer variables
//		m constant buffer variable data:
//			4 bytes - paramater offset
//			4 bytes - start offset
//			4 bytes - size
//
// 4 bytes - number of input layout items
// n Input layout data:
//		* - semantic name string (null terminated)
//		4 bytes - semantic index
//		4 bytes - dxgiFormat
//		4 bytes - itemSize
//		4 bytes - parameter offset
// 
// 4 bytes - number of other resources (currently textures and sampler states) 
//		4 bytes - resource type (D3D_SIT_TEXTURE and D3D_SIT_SAMPLER)
//		* - resource name (null terminated)
//
//
void ShaderReflector::ReflectShader(const wstring& path)
{
	wstring outputPath = path.substr(0, path.length() - 3) + L".shadermetadata";

	auto shaderBuffer = Tools::ReadFileToVector(path);
	auto metadataBuffer = ReflectShaderImpl(shaderBuffer);

	ofstream out(outputPath, ios::binary);
	Assert(out.is_open());

	out.write(reinterpret_cast<const char*>(metadataBuffer.data()), metadataBuffer.size());
	out.close();
}
