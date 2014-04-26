#include "PrecompiledHeader.h"
#include "Camera.h"
#include "Constants.h"
#include "Parameters.h"

Camera::Camera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight) :
	m_Position(0.0f, 0.0f, 0.0f),
	m_Rotation(0.0f, 0.0f, 0.0f),
	m_DirtyViewMatrix(true),
	m_DirtyViewProjectionMatrix(true)
{
	if (usePerspective)
	{
		m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovRH(fovY, aspectRatio, Constants::CameraNear, Constants::CameraFar);
		m_ProjectionMatrix = DirectX::XMMatrixTranspose(m_ProjectionMatrix);
	}
	else
	{
		DirectX::XMMATRIX orthoMatrix = DirectX::XMMatrixOrthographicRH(orthoWidth, orthoHeight, Constants::CameraNear, Constants::CameraFar); 
		DirectX::XMMATRIX fixTranslateMatrix = DirectX::XMMatrixTranslation(-orthoWidth / 2.0f, -orthoHeight / 2.0f, 0.0f);
		DirectX::XMMATRIX flipYMatrix = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);

		m_ProjectionMatrix = DirectX::XMMatrixTranspose(fixTranslateMatrix * flipYMatrix * orthoMatrix);
	}
}

Camera::~Camera()
{
}
	
void Camera::LookUp(float value)
{
	m_Rotation.x += value;
	CheckRotationBounds();

	m_DirtyViewMatrix = true;
}

void Camera::LookDown(float value)
{
	m_Rotation.x -= value;
	CheckRotationBounds();

	m_DirtyViewMatrix = true;
}

void Camera::LookLeft(float value)
{
	m_Rotation.y += value;
	m_DirtyViewMatrix = true;
}

void Camera::LookRight(float value)
{
	m_Rotation.y -= value;
	m_DirtyViewMatrix = true;
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

const DirectX::XMMATRIX& Camera::GetViewMatrix()
{
	if (m_DirtyViewMatrix)
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		DirectX::XMVECTOR up = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), rotationMatrix);
		DirectX::XMVECTOR lookAt = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f), rotationMatrix);

		lookAt = DirectX::XMVectorAdd(lookAt, DirectX::XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 1.0f));
	
		DirectX::XMVECTOR position = DirectX::XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 1.0f);
		m_ViewMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(position, lookAt, up));
		m_DirtyViewMatrix = false;
		m_DirtyViewProjectionMatrix = true;
	}

	return m_ViewMatrix;
}

const DirectX::XMMATRIX& Camera::GetViewProjectionMatrix()
{
	if (m_DirtyViewMatrix || m_DirtyViewProjectionMatrix)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * GetViewMatrix();
		m_DirtyViewProjectionMatrix = false;
	}

	return m_ViewProjectionMatrix;
}

void Camera::SetRenderParameters(RenderParameters& renderParameters)
{
	memcpy(&renderParameters.viewMatrix, &GetViewMatrix(), sizeof(DirectX::XMMATRIX));
	memcpy(&renderParameters.projectionMatrix, &m_ProjectionMatrix, sizeof(DirectX::XMMATRIX));
	memcpy(&renderParameters.viewProjectionMatrix, &GetViewProjectionMatrix(), sizeof(DirectX::XMMATRIX));

	renderParameters.cameraPosition = m_Position;
}