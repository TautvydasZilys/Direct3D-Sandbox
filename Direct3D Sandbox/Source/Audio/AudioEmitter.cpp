#include "PrecompiledHeader.h"
#include "AudioEmitter.h"
#include "Tools.h"

AudioEmitter::AudioEmitter(float innerRadius)
{
	ZeroMemory(&m_Emitter, sizeof(m_Emitter));

	m_Emitter.InnerRadius = innerRadius;
	m_Emitter.ChannelCount = 1;
	m_Emitter.CurveDistanceScaler = 1.0f;
	m_Emitter.DopplerScaler = 1.0f;

	Assert(sizeof(DirectX::XMFLOAT3) == sizeof(D3DVECTOR));
	memcpy(&m_Emitter.OrientFront, &DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), sizeof(DirectX::XMFLOAT3));
	memcpy(&m_Emitter.OrientTop, &DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), sizeof(DirectX::XMFLOAT3));
}

AudioEmitter::~AudioEmitter()
{
}

void AudioEmitter::SetPosition(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity)
{
	Assert(sizeof(DirectX::XMFLOAT3) == sizeof(D3DVECTOR));
	memcpy(&m_Emitter.Position, &position, sizeof(DirectX::XMFLOAT3));
	memcpy(&m_Emitter.Velocity, &velocity, sizeof(DirectX::XMFLOAT3));
}
