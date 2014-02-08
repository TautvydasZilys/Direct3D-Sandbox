#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "SamplerState.h"
#include "Tools.h"

unordered_map<string, ComPtr<ID3D11SamplerState>> SamplerState::s_SamplerStates;

#if DEBUG
bool SamplerState::s_Initialized = false;
#endif

void SamplerState::Initialize()
{
#if DEBUG
	Assert(s_Initialized == false);
	s_Initialized = true;
#endif

	HRESULT result;
	D3D11_SAMPLER_DESC samplerDescription;
	ComPtr<ID3D11SamplerState> samplerState;

	auto device = GetD3D11Device();

	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = 16;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescription.BorderColor[0] = 0;
	samplerDescription.BorderColor[1] = 0;
	samplerDescription.BorderColor[2] = 0;
	samplerDescription.BorderColor[3] = 0;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
	
	result = device->CreateSamplerState(&samplerDescription, &samplerState);
	Assert(result == S_OK);
	
	s_SamplerStates["clampsampler"] = samplerState;
	
	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = 16;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescription.BorderColor[0] = 0;
	samplerDescription.BorderColor[1] = 0;
	samplerDescription.BorderColor[2] = 0;
	samplerDescription.BorderColor[3] = 0;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
	
	result = device->CreateSamplerState(&samplerDescription, &samplerState);
	Assert(result == S_OK);

	s_SamplerStates["wrapsampler"] = samplerState;

	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = 16;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescription.BorderColor[0] = 0;
	samplerDescription.BorderColor[1] = 0;
	samplerDescription.BorderColor[2] = 0;
	samplerDescription.BorderColor[3] = 0;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
	
	result = device->CreateSamplerState(&samplerDescription, &samplerState);
	Assert(result == S_OK);

	s_SamplerStates["mirrorsampler"] = samplerState;

	samplerDescription.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = 1;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samplerDescription.BorderColor[0] = 1.0f;
	samplerDescription.BorderColor[1] = 1.0f;
	samplerDescription.BorderColor[2] = 1.0f;
	samplerDescription.BorderColor[3] = 1.0f;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
	
	result = device->CreateSamplerState(&samplerDescription, &samplerState);
	Assert(result == S_OK);

	s_SamplerStates["shadowmapsampler"] = samplerState;
}

ComPtr<ID3D11SamplerState> SamplerState::Get(const string& name)
{
#if DEBUG
	Assert(s_Initialized == true);
#endif

	auto samplerName = Tools::ToLower(name);
	auto sampler = s_SamplerStates.find(samplerName);
	Assert(sampler != s_SamplerStates.end());

	return sampler->second;
}