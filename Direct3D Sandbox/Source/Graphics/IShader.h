#pragma once

#include "RenderParameters.h"

class IShader
{
protected:
	IShader();

public:
	virtual ~IShader();
	virtual void Render(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters) = 0;
};

