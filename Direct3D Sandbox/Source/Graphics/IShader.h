#pragma once

#include "RenderParameters.h"

class IShader
{
protected:
	IShader();

public:
	virtual ~IShader();
	virtual void Render(const GlobalParameters& globalParameters, const ModelParameters& modelParameters) = 0;
};

