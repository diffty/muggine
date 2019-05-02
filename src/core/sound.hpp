#ifndef SOUND_HPP
#define SOUND_HPP

#if WITH_FMOD
#include <FMOD/fmod.hpp>
#endif

#include "../utils/linked_list.hpp"


class Sound
{
private:
#if WITH_FMOD
	FMOD::System* m_pSystem;
	FMOD::Channel* m_pChannel;
#endif

	LinkedList m_llSoundList;

	static Sound* s_pInstance;

public:
	Sound();
	~Sound();

	void addSound(char* szSoundPath, bool pLoop);
	void playSound(int iSoundId);
	void stopSound(int iSoundId);

	void update();

	void destroy();

	static Sound* get();
};


#endif