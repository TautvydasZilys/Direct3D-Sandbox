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
		DirectX::XMMATRIX orthoMatrix = DirectX::XMMatrixOrthographicRH(orthoWidth, orthoHeight, Constants::CameraNear, Constants::CameraFar); 
		DirectX::XMMATRIX fixTranslateMatrix = DirectX::XMMatrixTranslation(-orthoWidth / 2.0f, -orthoHeight / 2.0f, 0.0f);
		DirectX::XMMATRIX flipYMatrix = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);

		DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, 
			DirectX::XMMatrixTranspose(fixTranslateMatrix * flipYMatrix * orthoMatrix));
	}
}

Camera::~Camera()
{
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

void Camera::SetRenderParameters(RenderParameters& renderParameters)
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