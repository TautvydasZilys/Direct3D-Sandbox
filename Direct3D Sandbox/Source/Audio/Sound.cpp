#include "PrecompiledHeader.h"
#include "AudioEmitter.h"
#include "AudioManager.h"
#include "RiffFile.h"
#include "Sound.h"
#include "Tools.h"

Sound::Sound(const wstring& waveFilePath, bool loopForever, bool hasReverb) :
	m_SoundCallbacks(this),
	m_SubmixVoice(nullptr)
{
	auto waveFile = RiffFile::Create(waveFilePath);
	Assert(waveFile.GetFormat() == RiffFourCC::WAVE);

	ZeroMemory(&m_WaveFormat, sizeof(m_WaveFormat));
	ZeroMemory(&m_AudioBuffer, sizeof(m_AudioBuffer));

	const auto& formatChunk = waveFile.GetChunk(RiffFourCC::FMT);
	Assert(sizeof(m_WaveFormat) >= formatChunk.GetSize());
	memcpy(&m_WaveFormat, formatChunk.GetData(), formatChunk.GetSize());

	auto& dataChunk = waveFile.GetChunk(RiffFourCC::DATA);
	m_SoundDataBuffer = std::move(dataChunk.GetDataBuffer());

	m_AudioBuffer.AudioBytes = dataChunk.GetSize();
	m_AudioBuffer.pAudioData = m_SoundDataBuffer.get();
	m_AudioBuffer.Flags = XAUDIO2_END_OF_STREAM;
	m_AudioBuffer.LoopCount = loopForever ? XAUDIO2_LOOP_INFINITE : 0;

	if (hasReverb)
	{
		m_SubmixVoice = AudioManager::GetInstance().CreateSubmixVoice(m_WaveFormat);
	}
}

Sound::Sound(Sound&& other) :
	m_SoundDataBuffer(std::move(other.m_SoundDataBuffer)),
	m_AudioBuffer(other.m_AudioBuffer),
	m_WaveFormat(other.m_WaveFormat),
	m_Voices(std::move(other.m_Voices)),
	m_SoundCallbacks(this),
	m_SubmixVoice(other.m_SubmixVoice)
{
	other.m_SubmixVoice = nullptr;
	other.m_AudioBuffer.pAudioData = nullptr;
	other.m_AudioBuffer.pContext = nullptr;
}

Sound::~Sound()
{
	for (auto& voice : m_Voices)
	{
		voice.sourceVoice->DestroyVoice();
	}

	if (m_SubmixVoice != nullptr)
	{
		m_SubmixVoice->DestroyVoice();
	}
}

size_t Sound::GetNotPlayingVoiceIndex()
{
	auto voiceCount = m_Voices.size();

	for (size_t i = 0u; i < voiceCount; i++)
	{
		if (!m_Voices[i].isPlaying)
		{
			return i;
		}
	}

	// All voices are playing
	return CreateVoice();
}

size_t Sound::CreateVoice()
{
	auto sourceVoice = AudioManager::GetInstance().CreateSourceVoice(reinterpret_cast<WAVEFORMATEX*>(&m_WaveFormat), &m_SoundCallbacks, m_SubmixVoice);

	m_Voices.emplace_back(sourceVoice, false);
	return m_Voices.size() - 1;
}

Sound::Voice& Sound::GetVoiceForPlayback()
{
	HRESULT result;

	auto i = GetNotPlayingVoiceIndex();
	auto& voice = m_Voices[i];
	
	m_AudioBuffer.pContext = reinterpret_cast<void*>(i);
	result = voice.sourceVoice->SubmitSourceBuffer(&m_AudioBuffer);
	Assert(result == S_OK);
	
	return voice;
}

void Sound::PlayImpl(Voice& voiceToPlay)
{
	voiceToPlay.isPlaying = true;
	auto result = voiceToPlay.sourceVoice->Start();
	Assert(result == S_OK);
}

void Sound::Play()
{
	PlayImpl(GetVoiceForPlayback());
}

void Sound::Play3D(const AudioEmitter& audioEmitter)
{
	auto& audioManager = AudioManager::GetInstance();
	auto& voice = GetVoiceForPlayback();
	
	audioManager.Calculate3DAudioForVoice(audioEmitter.GetEmitter(), voice.sourceVoice, m_WaveFormat.Format.nChannels, m_SubmixVoice);
	PlayImpl(voice);
}