#pragma once

class AudioEmitter
{
private:
	X3DAUDIO_EMITTER m_Emitter;

public:
	AudioEmitter(float innerRadius);
	~AudioEmitter();

	const X3DAUDIO_EMITTER& GetEmitter() const { return m_Emitter; }
	void SetPosition(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity);
};