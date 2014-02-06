#include "PrecompiledHeader.h"
#include "Camera.h"
#include "Constants.h"
#include "Parameters.h"

Camera::Camera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight) :
	m_Position(0.0f, 0.0f, 0.0f),
	m_Rotation(0.0f, 0.0f, 0.0f)
{
	if (usePerspective)
	{
		DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, 
			DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovRH(fovY, aspectRatio, Constants::CameraNear, Constants::CameraFar)));
	}
	else
	{
		DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, 
			DirectX::XMMatrixTranspose(DirectX::XMMatrixOrthographicRH(orthoWidth, orthoHeight, Constants::CameraNear, Constants::CameraFar)));
	}
}

Camera::~Camera()
{
}

void Camera::GoForward(float value)
{
	m_Position.x -= value * cos(m_Rotation.x) * sin(m_Rotation.y);
	m_Position.y -= -value * sin(m_Rotation.x);
	m_Position.z -= value * cos(m_Rotation.x) * cos(m_Rotation.y);
}

void Camera::GoBack(float value)
{
	m_Position.x += value * cos(m_Rotation.x) * sin(m_Rotation.y);
	m_Position.y += -value * sin(m_Rotation.x);
	m_Position.z += value * cos(m_Rotation.x) * cos(m_Rotation.y);
}

void Camera::GoUp(float value)
{
	m_Position.y += value;
}

void Camera::GoDown(float value)
{
	m_Position.y -= value;
}

// Rotation matrix for only y rotation:
// 
//  cos(y)      0      sin(y)
//   0          0        0
// -sin(y)      0      cos(y)
//
void Camera::GoLeft(float value)
{
	m_Position.x -= value * cos(m_Rotation.y);
	m_Position.z -= -value * sin(m_Rotation.y);
}

void Camera::GoRight(float value)
{
	m_Position.x += value * cos(m_Rotation.y);
	m_Position.z += -value * sin(m_Rotation.y);
}
	
void Camera::LookUp(float value)
{
	m_Rotation.x += value;
	CheckRotationBounds();
}

void Camera::LookDown(float value)
{
	m_Rotation.x -= value;
	CheckRotationBounds();
}

void Camera::LookLeft(float value)
{
	m_Rotation.y += value;
}

void Camera::LookRight(float value)
{
	m_Rotation.y -= value;
}

void Camera::CheckRotationBounds()
{
	if (m_Rotation.x > DirectX::XM_PI / 2.0f)
	{
		m_Rotation.x = DirectX::XM_PI / 2.0f;
	}
	else if (m_Rotation.x < -DirectX::XM_PI / 2.0f)
	{
		m_Rotation.x = -DirectX::XM_PI / 2.0f;
	}
}

void Camera::SetRenderParameters(RenderParameters& renderParameters) const
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	DirectX::XMVECTOR up = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), rotationMatrix);
	DirectX::XMVECTOR lookAt = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f), rotationMatrix);

	lookAt = DirectX::XMVectorAdd(lookAt, DirectX::XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 1.0f));
	
	DirectX::XMVECTOR position = DirectX::XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 1.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtRH(position, lookAt, up);

	DirectX::XMStoreFloat4x4(&renderParameters.viewMatrix, DirectX::XMMatrixTranspose(viewMatrix));
	memcpy(&renderParameters.projectionMatrix, &m_ProjectionMatrix, sizeof(DirectX::XMFLOAT4X4));

	renderParameters.cameraPosition = m_Position;
}