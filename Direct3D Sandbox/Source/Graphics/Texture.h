#pragma once

class Texture
{
private:
	static unordered_map<wstring, ComPtr<ID3D11ShaderResourceView>> s_Textures;

	Texture();
	~Texture();

public:	
	static void LoadTexture(const wstring& path);
	static ComPtr<ID3D11ShaderResourceView> Get(const wstring& path);
};

