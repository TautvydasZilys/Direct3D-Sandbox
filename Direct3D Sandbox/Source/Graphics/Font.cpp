#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "Font.h"
#include "Tools.h"

unordered_map<wstring, Font> Font::s_FontCache;

// Assumes BGRA format pixel format
static unique_ptr<uint8_t[]> ParseFontPixels(const uint8_t* alphaPixels, int count)
{
	unique_ptr<uint8_t[]> rgbPixels(new uint8_t[4 * count]);

	for (int i = 0; i < count; i++)
	{
		if (alphaPixels[i] > 50)
		{
			rgbPixels[4 * i] = alphaPixels[i];
			rgbPixels[4 * i + 1] = alphaPixels[i];
			rgbPixels[4 * i + 2] = alphaPixels[i];
			rgbPixels[4 * i + 3] = 255;
		}
		else
		{
			rgbPixels[4 * i] = 0;
			rgbPixels[4 * i + 1] = 0;
			rgbPixels[4 * i + 2] = 0;
			rgbPixels[4 * i + 3] = 0;
		}
	}

	return rgbPixels;
}

Font::Font(const wstring& path)
{
	HRESULT result;
	ComPtr<ID3D11Texture2D> texture2D;
	D3D11_TEXTURE2D_DESC textureDescription;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescription;
	D3D11_SUBRESOURCE_DATA textureData;

	unsigned int position = 0;
	auto font = Tools::ReadFileToVector(path);
	
	auto fontWidth = Tools::BufferReader::ReadUInt(font, position);
	auto fontHeight = Tools::BufferReader::ReadUInt(font, position);

	textureDescription.Width = fontWidth;
	textureDescription.Height = fontHeight;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_IMMUTABLE;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;
	
	auto data = ParseFontPixels(font.data() + 8, static_cast<int>(textureDescription.Width * textureDescription.Height));
	textureData.pSysMem = data.get();
	textureData.SysMemPitch = textureDescription.Width * 4;
	textureData.SysMemSlicePitch = 0;

	result = GetD3D11Device()->CreateTexture2D(&textureDescription, &textureData, &texture2D);
	Assert(result == S_OK);

	srvDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescription.Texture2D.MostDetailedMip = 0;
	srvDescription.Texture2D.MipLevels = 1;
	
	result = GetD3D11Device()->CreateShaderResourceView(texture2D.Get(), &srvDescription, &m_FontTexture);
	Assert(result == S_OK);

	position = 8 + fontWidth * fontHeight;
	m_CharacterMetadata.resize(128);
	auto numberOfCharacters = Tools::BufferReader::ReadUInt(font, position);

	for (int i = 0; i < numberOfCharacters; i++)
	{
		auto character = Tools::BufferReader::ReadChar(font, position);
		auto aSpacing = Tools::BufferReader::ReadFloat(font, position);
		auto bSpacing = Tools::BufferReader::ReadFloat(font, position);
		auto cSpacing = Tools::BufferReader::ReadFloat(font, position);
		auto yOffset = Tools::BufferReader::ReadUInt(font, position);
		auto horizontalOffset = Tools::BufferReader::ReadUInt(font, position);

		if (m_CharacterMetadata.size() < character + 1)
		{
			m_CharacterMetadata.resize(character + 1);
		}

		m_CharacterMetadata[character] = CharacterMetadata(aSpacing, bSpacing, cSpacing, yOffset, horizontalOffset);
	}
}

Font::~Font()
{
}

void Font::LoadFont(const wstring& path)
{
	s_FontCache.emplace(Tools::ToLower(path), Font(path));
}

Font& Font::Get(const wstring& path)
{
	auto fontPath = Tools::ToLower(path);
	auto font = s_FontCache.find(fontPath);

	Assert(font != s_FontCache.end());
	return font->second;
}