#pragma once

#include "Camera.h"

class FPSControllerCamera :
	public Camera
{
public:
	FPSControllerCamera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight);
	virtual ~FPSControllerCamera();
	
	virtual void GoForward(float value);
	virtual void GoBack(float value);
	virtual void GoUp(float value);
	virtual void GoDown(float value);
	virtual void GoLeft(float value);
	virtual void GoRight(float value);
};