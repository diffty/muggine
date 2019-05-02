#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "../utils/jsonreader.hpp"
#include "../utils/linked_list.hpp"
#include "rsc_manager.hpp"
#include "../graphics/sprite.hpp"
#include "../graphics/animatedsprite.hpp"
#include "../graphics/scene.hpp"
#include "../animation/animationevent.hpp"
#include "../animation/animationtimeline.hpp"


typedef struct SceneDescription {
	Scene* pScene;
	AnimationTimeline* pAnimTimeline;
} SceneDescription;


class SceneManager {
public:
	SceneManager();
	~SceneManager();

	Scene* loadFromJSON(const char* szPath);
	IWidget* loadObject(Scene* pScene, char* szAsset, char* szName, char* szType);

	SceneDescription* getScene(char* szName);

private:
	LinkedList m_llSceneCollection;
};

#endif
