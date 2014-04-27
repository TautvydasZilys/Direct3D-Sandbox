#pragma once

struct VertexParameters;
struct RenderParameters;

enum ShaderType
{
	COLOR_SHADER = 0,
	TEXTURE_SHADER,
	LIGHTING_SHADER,
	NORMAL_MAP_SHADER,
	ANIMATION_NORMAL_MAP_SHADER,
	PLAYGROUND_SHADER,
	INFINITE_GROUND_SHADER,
	SHADER_COUNT
};

class IShader
{
protected:
	IShader();

private:
	static vector<shared_ptr<IShader>> s_Shaders;

	IShader(IShader& other);
	IShader& operator=(const IShader& other);

public:
	virtual ~IShader();
	
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(unsigned int vertexCount, D3D11_USAGE usage) const = 0;
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(unsigned int vertexCount, const VertexParameters vertices[], 
		D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE) const = 0;
	virtual void UploadVertexData(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, const VertexParameters vertices[]) const = 0;

	virtual void SetRenderParameters(const RenderParameters& renderParameters) = 0;
	virtual const unsigned int* GetInputLayoutSizePtr() const = 0;
	
	static void LoadShaders();
	static IShader& GetShader(ShaderType shaderType) { return *s_Shaders[shaderType]; }
};

