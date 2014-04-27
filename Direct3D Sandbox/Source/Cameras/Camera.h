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
	virtual ~Camera();
	
	void SetPosition(float x, float y, float z) { m_Position = DirectX::XMFLOAT3(x, y, z); }
	void SetPosition(DirectX::XMFLOAT3 position) { m_Position = position; }
	void SetRotation(float x, float y, float z) { m_Rotation = DirectX::XMFLOAT3(x, y, z); }
	void SetRotation(DirectX::XMFLOAT3 rotation) { m_Rotation = rotation; }

	const DirectX::XMFLOAT3& GetPosition() const { return m_Position; }
	const DirectX::XMFLOAT3& GetRotation() const { return m_Rotation; }
	
	virtual void GoForward(float value) {}
	virtual void GoBack(float value) {}
	virtual void GoUp(float value) {}
	virtual void GoDown(float value) {}
	virtual void GoLeft(float value) {}
	virtual void GoRight(float value) {}
	
	void LookUp(float value);
	void LookDown(float value);
	void LookLeft(float value);
	void LookRight(float value);

	virtual void SetRenderParameters(RenderParameters& renderParameters);
};

