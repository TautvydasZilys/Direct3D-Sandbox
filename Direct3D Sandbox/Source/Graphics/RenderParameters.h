#pragma once

#include "PrecompiledHeader.h"

#define GLOBAL_PARAMETERS \
			FIELD(float, time) \
			FIELD(DirectX::XMFLOAT4X4, projectionMatrix) \
			FIELD(DirectX::XMFLOAT4X4, viewMatrix)


#define MODEL_PARAMETERS \
			FIELD(DirectX::XMFLOAT4X4, worldMatrix)


#define VERTEX_PARAMETERS \
			FIELD(DirectX::XMFLOAT4, position) \
			FIELD(DirectX::XMFLOAT4, color) \
			FIELD(DirectX::XMFLOAT2, textureCoordinates) \
			FIELD(DirectX::XMFLOAT3, normal) \
			FIELD(DirectX::XMFLOAT3, tangent) \
			FIELD(DirectX::XMFLOAT3, binormal)



struct GlobalParameters
{
#define FIELD(type, name) type name;
	GLOBAL_PARAMETERS
#undef FIELD

	inline const uint8_t* GetField(const string& fieldName) const
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return reinterpret_cast<const uint8_t*>(&name);
		GLOBAL_PARAMETERS
#undef FIELD

		return nullptr;
	}

	inline static unsigned int GetFieldByteOffset(const string& fieldName)
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return offsetof(GlobalParameters, name);
		GLOBAL_PARAMETERS
#undef FIELD

		return -1;
	}
};

struct ModelParameters
{	
#define FIELD(type, name) type name;
	MODEL_PARAMETERS
#undef FIELD

	inline const uint8_t* GetField(const string& fieldName) const
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return reinterpret_cast<const uint8_t*>(&name);
		MODEL_PARAMETERS
#undef FIELD

		return nullptr;
	}

	inline static unsigned int GetFieldByteOffset(const string& fieldName)
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return offsetof(ModelParameters, name);
		MODEL_PARAMETERS
#undef FIELD

		return -1;
	}
};

struct VertexParameters
{	
#define FIELD(type, name) type name;
	VERTEX_PARAMETERS
#undef FIELD

	inline const uint8_t* GetField(const string& fieldName) const
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return reinterpret_cast<const uint8_t*>(&name);
		VERTEX_PARAMETERS
#undef FIELD

		return nullptr;
	}

	inline static unsigned int GetFieldByteOffset(const string& fieldName)
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return offsetof(VertexParameters, name);
		VERTEX_PARAMETERS
#undef FIELD

		return -1;
	}
};

