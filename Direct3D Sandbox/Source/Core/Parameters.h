#pragma once

#include "PrecompiledHeader.h"

#define RENDER_PARAMETERS \
			FIELD(float, time) \
			FIELD(float, frameTime) \
			FIELD(DirectX::XMFLOAT4X4, projectionMatrix) \
			FIELD(DirectX::XMFLOAT4X4, viewMatrix) \
			FIELD(DirectX::XMFLOAT4X4, worldMatrix) \
			FIELD(DirectX::XMFLOAT4X4, inversedTransposedWorldMatrix) \
			FIELD(DirectX::XMFLOAT4, color) \
			FIELD(DirectX::XMFLOAT3, lightDirection) \
			FIELD(DirectX::XMFLOAT3, lightColor) \
			FIELD(DirectX::XMFLOAT3, ambientColor) \
			FIELD(float, specularIntensity) \
			FIELD(DirectX::XMFLOAT2, groundScale) \
			FIELD(DirectX::XMFLOAT2, uvTiling) \
			FIELD(DirectX::XMFLOAT3, cameraPosition) \
			FIELD(DirectX::XMFLOAT4, padding) \
			FIELD(ID3D11ShaderResourceView*, texture) \
			FIELD(ID3D11ShaderResourceView*, normalMap) \
			FIELD(int, screenWidth) \
			FIELD(int, screenHeight)


#define VERTEX_PARAMETERS \
			FIELD(DirectX::XMFLOAT4, position) \
			FIELD(DirectX::XMFLOAT2, textureCoordinates) \
			FIELD(DirectX::XMFLOAT3, normal) \
			FIELD(DirectX::XMFLOAT3, tangent) \
			FIELD(DirectX::XMFLOAT3, binormal) \
			FIELD(DirectX::XMFLOAT4, position2) \
			FIELD(DirectX::XMFLOAT3, normal2) \
			FIELD(DirectX::XMFLOAT3, tangent2) \
			FIELD(DirectX::XMFLOAT3, binormal2)


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

		return 0xFFFFFFFF;
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

		return 0xFFFFFFFF;
	}

	inline bool operator==(const VertexParameters& other) const
	{
		return 
#define FIELD(type, name) memcmp(&name, &other.name, sizeof(type)) == 0 &&
			VERTEX_PARAMETERS
#undef FIELD
			true;
	};

private:
	VertexParameters(const VertexParameters& other);
	VertexParameters& operator=(const VertexParameters& other);
};

template<> 
struct hash<DirectX::XMFLOAT2>
{	
	size_t operator()(const DirectX::XMFLOAT2& value) const
	{
		return hash<float>()(value.x) ^ hash<float>()(value.y);
	}
};

template<> 
struct hash<DirectX::XMFLOAT3>
{	
	size_t operator()(const DirectX::XMFLOAT3& value) const
	{
		return hash<float>()(value.x) ^ hash<float>()(value.y) ^ hash<float>()(value.z);
	}
};

template<> 
struct hash<DirectX::XMFLOAT4>
{	
	size_t operator()(const DirectX::XMFLOAT4& value) const
	{
		return hash<float>()(value.x) ^ hash<float>()(value.y) ^ hash<float>()(value.z) ^ hash<float>()(value.w);
	}
};

template<> 
struct hash<VertexParameters>
{	
	size_t operator()(const VertexParameters& value) const
	{
		return
#define FIELD(type, name) hash<type>()(value.name) ^
			VERTEX_PARAMETERS
#undef FIELD
			0;
	}
};