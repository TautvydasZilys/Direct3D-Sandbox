#pragma once

#include "IModelInstance.h"

class PlayerInstance :
	public IModelInstance
{
public:
	PlayerInstance();
	virtual ~PlayerInstance();

	virtual void UpdateAndRender(RenderParameters& renderParameters);
};

