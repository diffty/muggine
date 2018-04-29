#include "sound.hpp"


Sound* Sound::s_pInstance = NULL;


Sound::Sound()
{
	s_pInstance = this;

	FMOD_RESULT result;
	unsigned int version;

	m_pChannel = 0;

	result = FMOD::System_Create(&m_pSystem);
	result = m_pSystem->getVersion(&version);

	result = m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	initList(&m_llSoundList);
}


Sound::~Sound() {
	m_pSystem->close();
}


void Sound::addSound(char* szSoundPath, bool pLoop) {
	FMOD::Sound* pNewSound;

	m_pSystem->createSound(szSoundPath, FMOD_DEFAULT, 0, &pNewSound);

	if (pLoop) {
		pNewSound->setMode(FMOD_LOOP_NORMAL);
	}
	else {
		pNewSound->setMode(FMOD_LOOP_OFF);
	}

	addDataToList(&m_llSoundList, pNewSound);
}

void Sound::playSound(int iSoundId) {
	FMOD::Sound* pSound = (FMOD::Sound*) getNodeInList(&m_llSoundList, iSoundId)->pData;
	m_pSystem->playSound(pSound, 0, false, &m_pChannel);
}

void Sound::stopSound(int iSoundId) {
	m_pChannel->setPaused(true);
}

void Sound::update() {
	m_pSystem->update();
}

void Sound::destroy() {
	LLNode* currNode = m_llSoundList.pHead;

	while (currNode != NULL) {
		((FMOD::Sound*) currNode->pData)->release();
		currNode = currNode->pNext;
	}

	clearList(&m_llSoundList);
}

Sound* Sound::get() {
	return s_pInstance;
}