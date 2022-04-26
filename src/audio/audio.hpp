#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "audiodata.hpp"

#include "../math/math.hpp"

class AudioBuffer {
public:
	AudioBuffer();
	~AudioBuffer();
};

class AudioSource {
public:
	AudioSource();
	~AudioSource();
};

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	void SetListener(Math::Vec3 _translation, Math::Vec3 _rotation);

protected:
	ALCdevice* m_ALCDev;
	ALCcontext* m_ALCCtx;
};