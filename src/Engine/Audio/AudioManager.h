
#ifndef ENGINE_AUDIO_AUDIOMANAGER_H
#define ENGINE_AUDIO_AUDIOMANAGER_H

class AudioManager
{
public:
	static AudioManager& getInstance() {
		static AudioManager instance;
		return instance;
	}
};

#endif // ENGINE_AUDIO_AUDIOMANAGER_H