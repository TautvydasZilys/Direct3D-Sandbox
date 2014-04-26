#pragma once

struct ModelData;
struct RenderParameters;

enum ShaderType
{
	COLOR_SHADER = 0,
	TEXTURE_SHADER,
	LIGHTING_SHADER,
	NORMAL_MAP_SHADER,
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
	
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(unsigned int vertexCount, const VertexParameters vertexData[]) const = 0;
	virtual void SetRenderParameters(const RenderParameters& renderParameters) = 0;
	virtual const unsigned int* GetInputLayoutSizePtr() const = 0;
	
	static void LoadShaders();
	static IShader& GetShader(ShaderType shaderType) { return *s_Shaders[shaderType]; }
};

