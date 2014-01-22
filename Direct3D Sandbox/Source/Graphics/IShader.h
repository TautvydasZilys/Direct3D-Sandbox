#pragma once

struct RenderParameters;

class IShader
{
protected:
	IShader();

public:
	virtual ~IShader();
	virtual void Render(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters) = 0;
};

