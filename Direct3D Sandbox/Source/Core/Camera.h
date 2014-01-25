#pragma once

struct RenderParameters;

class Camera
{
private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;

	void CheckRotationBounds();

	Camera(const Camera&);

public:
	Camera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight);
	~Camera();
	
	void SetPosition(float x, float y, float z) { m_Position = DirectX::XMFLOAT3(x, y, z); }
	void SetPosition(DirectX::XMFLOAT3 position) { m_Position = position; }
	void SetRotation(float x, float y, float z) { m_Rotation = DirectX::XMFLOAT3(x, y, z); }
	void SetRotation(DirectX::XMFLOAT3 rotation) { m_Rotation = rotation; }

	const DirectX::XMFLOAT3& GetPosition() const { return m_Position; }
	const DirectX::XMFLOAT3& GetRotation() const { return m_Rotation; }
	
	void GoForward(float value);
	void GoBack(float value);
	void GoUp(float value);
	void GoDown(float value);
	void GoLeft(float value);
	void GoRight(float value);
	
	void LookUp(float value);
	void LookDown(float value);
	void LookLeft(float value);
	void LookRight(float value);

	void SetRenderParameters(RenderParameters& renderParameters) const;
};

