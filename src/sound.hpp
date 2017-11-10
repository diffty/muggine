#ifndef SOUND_HPP
#define SOUND_HPP

#include <FMOD/fmod.hpp>
#include "linked_list.hpp"


class Sound
{
private:
	FMOD::System* m_pSystem;
	FMOD::Channel* m_pChannel = 0;

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