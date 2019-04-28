#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "jsonreader.hpp"
#include "linked_list.hpp"
#include "rsc_manager.hpp"
#include "sprite.hpp"
#include "animatedsprite.hpp"
#include "scene.hpp"
#include "animationevent.hpp"


class SceneManager {
public:
	SceneManager();
	~SceneManager();

	Scene* loadFromJSON(const char* szPath);
	IWidget* loadObject(Scene* pScene, char* szAsset, char* szName, char* szType);

	Scene* getScene(char* szName);

private:
	LinkedList m_llSceneCollection;
};

#endif
