#include "PrecompiledHeader.h"
#include "AudioEmitter.h"


AudioEmitter::AudioEmitter(float innerRadius)
{
	ZeroMemory(&m_Emitter, sizeof(m_Emitter));

	m_Emitter.InnerRadius = innerRadius;
	m_Emitter.ChannelCount = 1;
	m_Emitter.CurveDistanceScaler = 1.0f;
	m_Emitter.DopplerScaler = 1.0f;
	m_Emitter.OrientFront = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_Emitter.OrientTop = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
}

AudioEmitter::~AudioEmitter()
{
}

void AudioEmitter::SetPosition(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity)
{
	m_Emitter.Position = position;
	m_Emitter.Velocity = velocity;
}
