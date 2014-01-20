#pragma once

#include "PrecompiledHeader.h"

#define GLOBAL_PARAMETERS \
			FIELD(float, time) \
			FIELD(DirectX::XMFLOAT4X4, projectionMatrix) \
			FIELD(DirectX::XMFLOAT4X4, viewMatrix)


#define MODEL_PARAMETERS \
			FIELD(DirectX::XMFLOAT4X4, worldMatrix)


struct GlobalParameters
{
#define FIELD(type, name) type name;
	GLOBAL_PARAMETERS
#undef FIELD

	const void* GetField(const string& fieldName) const
	{
#define FIELD(type, name) if (fieldName == #name) return &name;
		GLOBAL_PARAMETERS
#undef FIELD

		return nullptr;
	}
};

struct ModelParameters
{	
#define FIELD(type, name) type name;
	MODEL_PARAMETERS
#undef FIELD

	const void* GetField(const string& fieldName) const
	{
#define FIELD(type, name) if (fieldName == #name) return &name;
		MODEL_PARAMETERS
#undef FIELD

		return nullptr;
	}
};

