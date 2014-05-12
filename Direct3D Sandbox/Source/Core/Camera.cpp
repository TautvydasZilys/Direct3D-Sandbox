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
		DirectX::XMMATRIX flipYMatrix = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);

		m_ProjectionMatrix = DirectX::XMMatrixTranspose(flipYMatrix * orthoMatrix);
	}
}

Camera::~Camera()
{
}

void Camera::RecalculateFrustumPlanes()
{
	DirectX::XMFLOAT4X4A vpm; // view projection matrix
	DirectX::XMStoreFloat4x4A(&vpm, m_ViewProjectionMatrix);

	m_FrustumPlanes[0] = DirectX::XMVectorSet(vpm._41 + vpm._11, vpm._42 + vpm._12, vpm._43 + vpm._13, vpm._44 + vpm._14); // Left plane
    m_FrustumPlanes[1] = DirectX::XMVectorSet(vpm._41 - vpm._11, vpm._42 - vpm._12, vpm._43 - vpm._13, vpm._44 - vpm._14); // Right plane
    m_FrustumPlanes[2] = DirectX::XMVectorSet(vpm._41 - vpm._21, vpm._42 - vpm._22, vpm._43 - vpm._23, vpm._44 - vpm._24); // Top plane
    m_FrustumPlanes[3] = DirectX::XMVectorSet(vpm._41 + vpm._21, vpm._42 + vpm._22, vpm._43 + vpm._23, vpm._44 + vpm._24); // Bottom plane
	m_FrustumPlanes[4] = DirectX::XMVectorSet(vpm._31, vpm._32, vpm._33, vpm._34); // Near plane
	m_FrustumPlanes[5] = DirectX::XMVectorSet(vpm._41 - vpm._31, vpm._42 - vpm._32, vpm._43 - vpm._33, vpm._44 - vpm._34); // Far plane

	for (int i = 0; i < 6; i++)
	{
		m_FrustumPlanes[i] = DirectX::XMVector3Normalize(m_FrustumPlanes[i]);
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
		
		RecalculateFrustumPlanes();
	}

	return m_ViewProjectionMatrix;
}

void Camera::SetRenderParameters(RenderParameters& renderParameters)
{
	memcpy(&renderParameters.viewMatrix, &GetViewMatrix(), sizeof(DirectX::XMMATRIX));
	memcpy(&renderParameters.projectionMatrix, &m_ProjectionMatrix, sizeof(DirectX::XMMATRIX));
	memcpy(&renderParameters.viewProjectionMatrix, &GetViewProjectionMatrix(), sizeof(DirectX::XMMATRIX));
	memcpy(renderParameters.frustumPlanes, m_FrustumPlanes, 6 * sizeof(DirectX::XMVECTOR));

	renderParameters.cameraPosition = m_Position;
}