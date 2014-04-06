#pragma once
#include "Tools.h"
#include "IShader.h"

class Model;
struct RenderParameters;

class Font
{
private:
	struct CharacterMetadata
	{		
		unsigned int xOffset;
		unsigned int yOffset;
		unsigned int characterWidth;
		unsigned int characterHeight;

		CharacterMetadata() {}

		CharacterMetadata(unsigned int xOffset, unsigned int yOffset, unsigned int characterWidth, unsigned int characterHeight) :
			xOffset(xOffset),
			yOffset(yOffset),
			characterWidth(characterWidth),
			characterHeight(characterHeight)
		{
		}
	};

	struct TextId
	{
		const string path;
		const IShader& shader;

		TextId(const string& path, const IShader& shader) : path(Tools::ToLower(path)), shader(shader) { }
		bool operator==(const TextId& other) const { return path == other.path && (&shader == &other.shader); }
	};

	struct TextIdHash
	{
		size_t operator()(const TextId& value) const
		{
			return hash<string>()(value.path) ^ hash<const void*>()(&value.shader);
		}
	};

	static unordered_map<wstring, Font> s_FontCache;
	
	int m_FontTextureWidth;
	int m_FontTextureHeight;
	ComPtr<ID3D11ShaderResourceView> m_FontTexture;
	vector<CharacterMetadata> m_CharacterMetadata;
	unsigned int m_LineSpacing;
	
	unordered_map<TextId, Model, TextIdHash> m_TextCache;
	
	Font(const wstring& path);
	~Font();
	
	Font(const Font& other);														// Not implemented (no copying allowed)
	Font& operator=(const Font& other);												// Not implemented (no copying allowed)
	Font(Font&& other);

	Model CreateTextModel(const string& text, IShader& shader);

	static Font* s_DefaultFont;

	template <typename _Ty1, typename _Ty2>
	friend struct pair;

public:	
	static void LoadFont(const wstring& path);
	static Font& Get(const wstring& path);
	static Font& GetDefault();
	
	static void SetDefault(const wstring& path);

	void DrawText(const string& text, int posX, int posY, RenderParameters& renderParameters, bool useCaching = false, IShader& shader = IShader::GetShader(ShaderType::TEXTURE_SHADER));
};

