#pragma once

#include "PrecompiledHeader.h"

struct GlobalParameters
{
	float time;
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
};

struct ModelParameters
{
	DirectX::XMFLOAT3 worldMatrix;
};

