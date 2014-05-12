#pragma once

#include "AlignedClass.h"

struct RenderParameters;

class Camera :
	public AlignedClass
{
protected:
	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;
	DirectX::XMMATRIX m_ViewProjectionMatrix;
	DirectX::XMVECTOR m_FrustumPlanes[6];
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;
	
	bool m_DirtyViewMatrix;
	bool m_DirtyViewProjectionMatrix;

	void CheckRotationBounds();
	void RecalculateFrustumPlanes();
	const DirectX::XMMATRIX& GetViewMatrix();
	const DirectX::XMMATRIX& GetViewProjectionMatrix();

	Camera(const Camera&);

public:
	Camera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight);
	~Camera();
	
	inline void SetPosition(float x, float y, float z) { m_Position = DirectX::XMFLOAT3(x, y, z); }
	inline void SetPosition(const DirectX::XMFLOAT3& position) { m_Position = position; m_DirtyViewMatrix = true; }
	inline void SetRotation(float x, float y, float z) { m_Rotation = DirectX::XMFLOAT3(x, y, z); }
	inline void SetRotation(const DirectX::XMFLOAT3& rotation) { m_Rotation = rotation; m_DirtyViewMatrix = true; }

	inline const DirectX::XMFLOAT3& GetPosition() const { return m_Position; }
	inline const DirectX::XMFLOAT3& GetRotation() const { return m_Rotation; }
	
	void SetRenderParameters(RenderParameters& renderParameters);
};

