#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "Font.h"
#include "IShader.h"
#include "Model.h"
#include "Tools.h"

unordered_map<wstring, Font> Font::s_FontCache;

Font::Font(const wstring& path)
{
	HRESULT result;
	ComPtr<ID3D11Texture2D> texture2D;
	D3D11_TEXTURE2D_DESC textureDescription;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescription;
	D3D11_SUBRESOURCE_DATA textureData;

	unsigned int position = 0;
	auto font = Tools::ReadFileToVector(path);
	
	m_FontTextureWidth = Tools::BufferReader::ReadUInt(font, position);
	m_FontTextureHeight = Tools::BufferReader::ReadUInt(font, position);

	textureDescription.Width = m_FontTextureWidth;
	textureDescription.Height = m_FontTextureHeight;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_IMMUTABLE;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;
	
	textureData.pSysMem = font.data() + 8;
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

	position = 8 + 4 * m_FontTextureWidth * m_FontTextureHeight;
	m_CharacterMetadata.resize(128);
	ZeroMemory(m_CharacterMetadata.data(), m_CharacterMetadata.size() * sizeof(CharacterMetadata));
	auto numberOfCharacters = Tools::BufferReader::ReadUInt(font, position);

	for (auto i = 0u; i < numberOfCharacters; i++)
	{
		auto character = Tools::BufferReader::ReadChar(font, position);
		auto xOffset = Tools::BufferReader::ReadUInt(font, position);
		auto yOffset = Tools::BufferReader::ReadUInt(font, position);
		auto characterWidth = Tools::BufferReader::ReadUInt(font, position);
		auto characterHeight = Tools::BufferReader::ReadUInt(font, position);

		if (static_cast<int>(m_CharacterMetadata.size()) < character + 1)
		{
			m_CharacterMetadata.resize(character + 1);
		}

		m_CharacterMetadata[character] = CharacterMetadata(xOffset, yOffset, characterWidth, characterHeight);
	}

	m_LineSpacing = Tools::BufferReader::ReadUInt(font, position);
}

Font::Font(Font&& other) :
	m_FontTextureWidth(other.m_FontTextureWidth),
	m_FontTextureHeight(other.m_FontTextureHeight),
	m_FontTexture(other.m_FontTexture),
	m_CharacterMetadata(std::move(other.m_CharacterMetadata)),
	m_LineSpacing(other.m_LineSpacing),
	m_TextCache(std::move(other.m_TextCache))
{
	other.m_FontTexture = nullptr;
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

Model Font::CreateTextModel(const string& text, IShader& shader)
{
	// 6 vertices per character
	ModelData modelData;
	float currentPosX = 0, currentPosY = 0;

	modelData.indexCount = 0;
	modelData.vertexCount = 6 * text.length();
	modelData.vertices = unique_ptr<VertexParameters[]>(new VertexParameters[6 * text.length()]);
	
	ZeroMemory(modelData.vertices.get(), sizeof(VertexParameters) * 6 * text.length());

	for (auto i = 0u; i < text.length(); i++)
	{
		for (auto u = 0; u < 6; u++)
		{
			modelData.vertices[6 * i + u].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			modelData.vertices[6 * i + u].position.w = 1.0f;
		}
	}

	for (auto i = 0u; i < text.length(); i++)
	{
		float startX, startY, endX, endY;
		float texStartX, texStartY, texEndX, texEndY;
		
		startX = currentPosX;
		endX = startX + m_CharacterMetadata[text[i]].characterWidth;
		startY = currentPosY;
		endY = startY + m_CharacterMetadata[text[i]].characterHeight;
		
		texStartX = static_cast<float>(m_CharacterMetadata[text[i]].xOffset) / static_cast<float>(m_FontTextureWidth);
		texStartY = static_cast<float>(m_CharacterMetadata[text[i]].yOffset) / static_cast<float>(m_FontTextureHeight);
		texEndX = static_cast<float>(m_CharacterMetadata[text[i]].xOffset + m_CharacterMetadata[text[i]].characterWidth) / static_cast<float>(m_FontTextureWidth);
		texEndY = static_cast<float>(m_CharacterMetadata[text[i]].yOffset + m_CharacterMetadata[text[i]].characterHeight) / static_cast<float>(m_FontTextureHeight);

		modelData.vertices[6 * i].position.x = startX;
		modelData.vertices[6 * i].position.y = startY;
		modelData.vertices[6 * i].textureCoordinates.x = texStartX;
		modelData.vertices[6 * i].textureCoordinates.y = texStartY;
		
		modelData.vertices[6 * i + 1].position.x = endX;
		modelData.vertices[6 * i + 1].position.y = startY;
		modelData.vertices[6 * i + 1].textureCoordinates.x = texEndX;
		modelData.vertices[6 * i + 1].textureCoordinates.y = texStartY;

		modelData.vertices[6 * i + 2].position.x = startX;
		modelData.vertices[6 * i + 2].position.y = endY;
		modelData.vertices[6 * i + 2].textureCoordinates.x = texStartX;
		modelData.vertices[6 * i + 2].textureCoordinates.y = texEndY;

		modelData.vertices[6 * i + 3].position.x = endX;
		modelData.vertices[6 * i + 3].position.y = startY;
		modelData.vertices[6 * i + 3].textureCoordinates.x = texEndX;
		modelData.vertices[6 * i + 3].textureCoordinates.y = texStartY;
		
		modelData.vertices[6 * i + 4].position.x = endX;
		modelData.vertices[6 * i + 4].position.y = endY;
		modelData.vertices[6 * i + 4].textureCoordinates.x = texEndX;
		modelData.vertices[6 * i + 4].textureCoordinates.y = texEndY;

		modelData.vertices[6 * i + 5].position.x = startX;
		modelData.vertices[6 * i + 5].position.y = endY;
		modelData.vertices[6 * i + 5].textureCoordinates.x = texStartX;
		modelData.vertices[6 * i + 5].textureCoordinates.y = texEndY;

		currentPosX = endX;

		if (text[i] == '\n')
		{
			currentPosX = 0;
			currentPosY += m_LineSpacing;
		}
	}

	return Model::CreateNonCachedModel(modelData, shader);
}

void Font::DrawText(const string& text, int posX, int posY, RenderParameters& renderParameters, bool useCaching, IShader& shader)
{	
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(static_cast<float>(posX), static_cast<float>(posY), 0.0f);

	DirectX::XMStoreFloat4x4(&renderParameters.worldMatrix, DirectX::XMMatrixTranspose(worldMatrix));
	DirectX::XMStoreFloat4x4(&renderParameters.inversedTransposedWorldMatrix, DirectX::XMMatrixInverse(nullptr, worldMatrix));
	renderParameters.texture = m_FontTexture.Get();

	if (!useCaching)
	{
		CreateTextModel(text, shader).Render(renderParameters);
	}
	else
	{
		auto buffer = m_TextCache.find(TextId(text, ref(shader)));

		if (buffer == m_TextCache.end())
		{
			m_TextCache.emplace(TextId(text, ref(shader)), CreateTextModel(text, shader));
			buffer = m_TextCache.find(TextId(text, ref(shader)));
		}

		Assert(buffer != m_TextCache.end());
		buffer->second.Render(renderParameters);
	}
}

