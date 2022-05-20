#pragma once

#include "audiodata.hpp"

#include "../../common/math/math.hpp"

struct AudioSourceImpl;
class AudioSource {
public:
	AudioSource();
	~AudioSource();

	void SetData(const AudioData& _data);

	void SetLooped(bool _looped);

	bool IsLooped() const;

	void Play();
	void Stop();
	void Pause();

protected:
	// ALuint m_ALBuffer;
	// ALuint m_ALSource;
	AudioSourceImpl* m_Impl;
	bool m_Looped;
};

struct AudioManagerImpl;
class AudioManager {
public:
	AudioManager();
	~AudioManager();

	void SetListener(Math::Vec3 _translation, Math::Vec3 _rotation);

protected:
	// ALCdevice* m_ALCDev;
	// ALCcontext* m_ALCCtx;
	AudioManagerImpl* m_Impl;
};