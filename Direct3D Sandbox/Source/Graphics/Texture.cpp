#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "Source\External\DirectXTK\DDSTextureLoader.h"
#include "Texture.h"
#include "Tools.h"

unordered_map<wstring, ComPtr<ID3D11ShaderResourceView>> Texture::s_Textures;

void Texture::LoadTexture(const wstring& path)
{
	HRESULT result;
	ComPtr<ID3D11ShaderResourceView> texture;

	result = DirectX::CreateDDSTextureFromFile(GetD3D11Device(), path.c_str(), nullptr, &texture);
	Assert(result == S_OK);

	s_Textures.insert(make_pair(Tools::ToLower(path), texture));
}

ComPtr<ID3D11ShaderResourceView> Texture::Get(const wstring& path)
{
	auto texturePath = Tools::ToLower(path);
	auto texture = s_Textures.find(texturePath);

	Assert(texture != s_Textures.end());
	return texture->second;
}