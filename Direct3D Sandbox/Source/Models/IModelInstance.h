#pragma once

class RenderParameters;
class IModelInstance
{
private:
	IModelInstance(const IModelInstance& other);				// Not implement (copying is not allowed)
	IModelInstance& operator=(const IModelInstance& other);		// Not implement (copying is not allowed)

public:
	IModelInstance();
	virtual ~IModelInstance();
	
	virtual void UpdateAndRender(RenderParameters& renderParameters) = 0;
};
