#pragma once

#include "PrecompiledHeader.h"

#define RENDER_PARAMETERS \
			FIELD(float, time) \
			FIELD(DirectX::XMFLOAT4X4, projectionMatrix) \
			FIELD(DirectX::XMFLOAT4X4, viewMatrix) \
			FIELD(DirectX::XMFLOAT4X4, worldMatrix)


#define VERTEX_PARAMETERS \
			FIELD(DirectX::XMFLOAT4, position) \
			FIELD(DirectX::XMFLOAT4, color) \
			FIELD(DirectX::XMFLOAT2, textureCoordinates) \
			FIELD(DirectX::XMFLOAT3, normal) \
			FIELD(DirectX::XMFLOAT3, tangent) \
			FIELD(DirectX::XMFLOAT3, binormal)



struct RenderParameters
{
#define FIELD(type, name) type name;
	RENDER_PARAMETERS
#undef FIELD

	RenderParameters() {}

	inline const uint8_t* GetField(const string& fieldName) const
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return reinterpret_cast<const uint8_t*>(&name);
		RENDER_PARAMETERS
#undef FIELD

		return nullptr;
	}

	inline static unsigned int GetFieldByteOffset(const string& fieldName)
	{
#define FIELD(type, name) if (_stricmp(fieldName.c_str(), #name) == 0) return offsetof(RenderParameters, name);
		RENDER_PARAMETERS
#undef FIELD

		return -1;
	}

private:
	RenderParameters(const RenderParameters& other);
	RenderParameters& operator=(const RenderParameters& other);
};

struct VertexParameters
{	
#define FIELD(type, name) type name;
	VERTEX_PARAMETERS
#undef FIELD

	VertexParameters() {}

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

private:
	VertexParameters(const VertexParameters& other);
	VertexParameters& operator=(const VertexParameters& other);
};

