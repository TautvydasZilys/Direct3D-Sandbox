#include "PrecompiledHeader.h"
#include "AudioManager.h"
#include "Sound.h"
#include "Tools.h"

unique_ptr<AudioManager> AudioManager::s_Instance;

static const float kSpeedOfSound = X3DAUDIO_SPEED_OF_SOUND;
static const int k3DAudioCalculationFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB;

void AudioManager::Initialize()
{
	Assert(s_Instance == nullptr);
	s_Instance = unique_ptr<AudioManager>(new AudioManager);
}

AudioManager& AudioManager::GetInstance()
{
	Assert(s_Instance != nullptr);
	return *s_Instance;
}

AudioManager::AudioManager()
{
	// Init XAudio2

	auto result = XAudio2Create(&m_XAudio2);
	Assert(result == S_OK);

#if DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debugConfig;
	ZeroMemory(&debugConfig, sizeof(XAUDIO2_DEBUG_CONFIGURATION));

	debugConfig.TraceMask = XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_DETAIL;
	debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
	debugConfig.LogFileline = TRUE;
	debugConfig.LogFunctionName = TRUE;
	debugConfig.LogThreadID = TRUE;
	debugConfig.LogTiming = TRUE;

	m_XAudio2->SetDebugConfiguration(&debugConfig);
#endif

	result = m_XAudio2->CreateMasteringVoice(&m_MasteringVoice);
	Assert(result == S_OK);

	// Init X3DAudio

	DWORD channelMask;
	result = m_MasteringVoice->GetChannelMask(&channelMask);
	Assert(result == S_OK);

	m_MasteringVoice->GetVoiceDetails(&m_VoiceDetails);

	X3DAudioInitialize(channelMask, kSpeedOfSound, m_X3DAudio);
	ZeroMemory(&m_Listener, sizeof(m_Listener));
	ZeroMemory(&m_X3DSettings, sizeof(m_X3DSettings));
		
	m_X3DSettings.SrcChannelCount = 1;
	m_X3DSettings.DstChannelCount = m_VoiceDetails.InputChannels;
	m_3DAudioMatrixCoeficients = unique_ptr<FLOAT32[]>(new FLOAT32[2 * m_VoiceDetails.InputChannels]);
	m_X3DSettings.pMatrixCoefficients = m_3DAudioMatrixCoeficients.get();
}

AudioManager::~AudioManager()
{
	m_CachedSounds.clear();

	if (m_MasteringVoice != nullptr)
	{
		m_MasteringVoice->DestroyVoice();
	}
}

IXAudio2SubmixVoice* AudioManager::CreateSubmixVoice(const WAVEFORMATEXTENSIBLE& waveFormat)
{
	HRESULT result;
	IXAudio2SubmixVoice* submixVoice;
	IUnknown* reverbEffect;
	XAUDIO2_EFFECT_DESCRIPTOR effectDescriptor;	
	XAUDIO2_EFFECT_CHAIN effectChain;

	result = XAudio2CreateReverb(&reverbEffect);
	Assert(result == S_OK);

	effectDescriptor.InitialState = true;
	effectDescriptor.OutputChannels = 1;
	effectDescriptor.pEffect = reverbEffect;
	
	effectChain.EffectCount = 1;
	effectChain.pEffectDescriptors = &effectDescriptor;

	result = m_XAudio2->CreateSubmixVoice(&submixVoice, waveFormat.Format.nChannels, m_VoiceDetails.InputSampleRate, 0, 0, nullptr, &effectChain);
	Assert(result == S_OK);

	reverbEffect->Release();
	return submixVoice;
}

IXAudio2SourceVoice* AudioManager::CreateSourceVoice(const WAVEFORMATEX* waveFormat, IXAudio2VoiceCallback* voiceCallback, IXAudio2SubmixVoice* submixVoice)
{
	HRESULT result;
	IXAudio2SourceVoice* sourceVoice;

	if (submixVoice != nullptr)
	{
		XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2] = 
		{
			{ 0, submixVoice }, 
			{ 0, m_MasteringVoice }
		};

		XAUDIO2_VOICE_SENDS soundSendList = { 2, sendDescriptors };

		result = m_XAudio2->CreateSourceVoice(&sourceVoice, waveFormat, XAUDIO2_VOICE_USEFILTER, XAUDIO2_DEFAULT_FREQ_RATIO, voiceCallback, &soundSendList);
	}
	else
	{
		result = m_XAudio2->CreateSourceVoice(&sourceVoice, waveFormat, XAUDIO2_VOICE_USEFILTER, XAUDIO2_DEFAULT_FREQ_RATIO, voiceCallback);
	}

	Assert(result == S_OK);
	return sourceVoice;
}

void AudioManager::SetListenerPosition(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity, 
									   const DirectX::XMFLOAT3& front, const DirectX::XMFLOAT3& up)
{
	m_Listener.Position = position;
	m_Listener.Velocity = velocity;
	m_Listener.OrientFront = front;
	m_Listener.OrientTop = up;
}

void AudioManager::Calculate3DAudioForVoice(const X3DAUDIO_EMITTER& audioEmitter, IXAudio2SourceVoice* sourceVoice, int sourceChannels,
											IXAudio2SubmixVoice* submixVoice)
{
	X3DAudioCalculate(m_X3DAudio, &m_Listener, &audioEmitter, k3DAudioCalculationFlags, &m_X3DSettings);
	
	XAUDIO2_FILTER_PARAMETERS filterParameters = 
	{ 
		LowPassFilter, 
		2.0f * sinf(X3DAUDIO_PI / 6.0f * m_X3DSettings.LPFDirectCoefficient),
		1.0f
	};

	HRESULT result;
	
	for (int i = static_cast<int>(m_VoiceDetails.InputChannels - 1); i > -1; i--)
	{
		for (int j = 0; j < sourceChannels; j++)
		{
			m_X3DSettings.pMatrixCoefficients[i * sourceChannels + j] = m_X3DSettings.pMatrixCoefficients[i];
		}
	}

	result = sourceVoice->SetOutputMatrix(m_MasteringVoice, sourceChannels, m_VoiceDetails.InputChannels, m_X3DSettings.pMatrixCoefficients);
	Assert(result == S_OK);

	result = sourceVoice->SetFrequencyRatio(m_X3DSettings.DopplerFactor);
	Assert(result == S_OK);

	result = sourceVoice->SetFilterParameters(&filterParameters);
	Assert(result == S_OK);

	if (submixVoice != nullptr)
	{
		result = sourceVoice->SetOutputMatrix(submixVoice, sourceChannels, sourceChannels, m_X3DSettings.pMatrixCoefficients);
		Assert(result == S_OK);
	}
}

Sound& AudioManager::GetCachedSound(const wstring& path, bool loopForever, bool hasReverb)
{
	SoundCacheKey key(path, loopForever, hasReverb);
	auto& sound = s_Instance->m_CachedSounds.find(key);

	if (sound == s_Instance->m_CachedSounds.end())
	{
		s_Instance->m_CachedSounds.emplace(key, Sound(path, loopForever, hasReverb));
		sound = s_Instance->m_CachedSounds.find(key);
	}

	return sound->second;
}