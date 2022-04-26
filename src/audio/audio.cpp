#include "audio.hpp"

#include <stdio.h>

AudioManager::AudioManager() {
	m_ALCDev = alcOpenDevice(nullptr);
	if(m_ALCDev == nullptr) {
		fprintf(stderr, "Failed to initialise OpenAL!\n");
		return;
	}

	m_ALCCtx = alcCreateContext(m_ALCDev, nullptr);
	alcMakeContextCurrent(m_ALCCtx);
	if(m_ALCCtx == nullptr) {
		fprintf(stderr, "Failed to create an OpenAL context!\n");
		return;
	}
}
AudioManager::~AudioManager() {
	alcDestroyContext(m_ALCCtx);
	alcCloseDevice(m_ALCDev);
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

