#include "audio.hpp"

#include <stdio.h>

#include <AL/al.h>
#include <AL/alc.h>

struct AudioSourceImpl {
	ALuint m_ALBuffer;
	ALuint m_ALSource;
};

AudioSource::AudioSource() {
	m_Impl = new AudioSourceImpl;
	alGenBuffers(1, &m_Impl->m_ALBuffer);
	alGenSources(1, &m_Impl->m_ALSource);

	SetLooped(false);
}
AudioSource::~AudioSource() {
	alDeleteSources(1, &m_Impl->m_ALSource);
	alDeleteBuffers(1, &m_Impl->m_ALBuffer);
	delete m_Impl;
}

void AudioSource::SetData(const AudioData& _data) {
	alBufferData(m_Impl->m_ALBuffer, _data.format, _data.data.data(), _data.data.size(), _data.frequency);
	alSourcei(m_Impl->m_ALSource, AL_BUFFER, m_Impl->m_ALBuffer);
}

void AudioSource::SetLooped(bool _looped) {
	m_Looped = _looped;
	alSourcei(m_Impl->m_ALSource, AL_LOOPING, m_Looped);
}

bool AudioSource::IsLooped() const {
	return m_Looped;
}

void AudioSource::Play() {
	alSourcePlay(m_Impl->m_ALSource);
}

void AudioSource::Stop() {
	alSourceStop(m_Impl->m_ALSource);
}

void AudioSource::Pause() {
	alSourcePause(m_Impl->m_ALSource);
}



struct AudioManagerImpl {
	ALCdevice* m_ALCDev;
	ALCcontext* m_ALCCtx;
};

AudioManager::AudioManager() {
	m_Impl = new AudioManagerImpl;
	
	m_Impl->m_ALCDev = alcOpenDevice(nullptr);
	if(m_Impl->m_ALCDev == nullptr) {
		fprintf(stderr, "Failed to initialise OpenAL!\n");
		return;
	}

	m_Impl->m_ALCCtx = alcCreateContext(m_Impl->m_ALCDev, nullptr);
	alcMakeContextCurrent(m_Impl->m_ALCCtx);
	if(m_Impl->m_ALCCtx == nullptr) {
		fprintf(stderr, "Failed to create an OpenAL context!\n");
		return;
	}
}
AudioManager::~AudioManager() {
	alcDestroyContext(m_Impl->m_ALCCtx);
	alcCloseDevice(m_Impl->m_ALCDev);
	delete m_Impl;
}

void AudioManager::SetListener(Math::Vec3 _translation, Math::Vec3 _rotation) {
	Math::Mat4 rotMatrix = Math::Mat4::Inverse(
		Math::Mat4::RotateY(_rotation.y) * 
		Math::Mat4::RotateX(_rotation.x)
	);

	Math::Vec4 up = rotMatrix * Math::Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Math::Vec4 front = rotMatrix * Math::Vec4(0.0f, 0.0f, -1.0f, 1.0f);
	
	Math::Vec3 upNorm = Math::Vec3::Normalise(Math::Vec3(up.x,up.y,up.z));
	Math::Vec3 frNorm = Math::Vec3::Normalise(Math::Vec3(front.x,front.y,front.z));

	float ori[6] = {
		frNorm.x, frNorm.y, frNorm.z,
		upNorm.x, upNorm.y, upNorm.z
	};

	alListenerfv(AL_ORIENTATION, ori);
	alListener3f(AL_POSITION, _translation.x, _translation.y, _translation.z);
}

