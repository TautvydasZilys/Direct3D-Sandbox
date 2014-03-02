#pragma once

class Font
{
private:
	struct CharacterMetadata
	{
		float aSpacing;
		float bSpacing;
		float cSpacing;
		
		unsigned int yOffset;
		unsigned int horizontalOffset;

		CharacterMetadata(float aSpacing, float bSpacing, float cSpacing, unsigned int yOffset, unsigned int horizontalOffset) :
			aSpacing(aSpacing), 
			bSpacing(bSpacing),
			cSpacing(cSpacing),
			yOffset(yOffset),
			horizontalOffset(horizontalOffset)
		{
		}
	};

	static unordered_map<wstring, Font> s_FontCache;

	ComPtr<ID3D11ShaderResourceView> m_FontTexture;
	vector<CharacterMetadata> m_CharacterMetadata;

	Font(const wstring& path);
	~Font();
	
	Font(const Font& other);

public:	
	static void LoadFont(const wstring& path);
	static Font& Get(const wstring& path);

	ID3D11ShaderResourceView* GetFontTexture() { return m_FontTexture.Get(); }
};

