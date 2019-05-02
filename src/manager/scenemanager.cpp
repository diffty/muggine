#include "scenemanager.hpp"


SceneManager::SceneManager() {
	initList(&m_llSceneCollection);
}

SceneManager::~SceneManager() {

}

Scene* SceneManager::loadFromJSON(const char* szPath) {
	SceneDescription* pSceneDescription = new SceneDescription;

	JSONReader json(szPath);

	char* szSceneName = json.get<char*>("name");

	Scene* pNewScene = new Scene(szSceneName);
	addDataToList(&m_llSceneCollection, pSceneDescription);

	// Loading objects in scene
	JSONItem* pObjectsListNode = json.get<JSONItem*>("objects");
	for (int i = 0; i < pObjectsListNode->m_contentList.size(); i++) {
		JSONItem* pObjectInfo = (JSONItem*)pObjectsListNode->m_contentList[i]->pValue;
		char* szAsset = pObjectInfo->get<char*>("asset");
		char* szName = pObjectInfo->get<char*>("name");
		char* szType = pObjectInfo->get<char*>("type");
		JSONItem* pPosition = pObjectInfo->get<JSONItem*>("position");

		IWidget* pNewObject = loadObject(pNewScene, szAsset, szName, szType);

		if (pPosition != NULL) {
			int* iPosX = pPosition->get<int*>("x");
			int* iPosY = pPosition->get<int*>("y");
			pNewObject->translate(*iPosX, *iPosY);
		}
	}
	
	AnimationTimeline* pAnimationTimeline = new AnimationTimeline();

	// Loading animations
	// TODO: Sort them by chronological order
	JSONItem* pAnimationListNode = json.get<JSONItem*>("animations");
	for (int i = 0; i < pAnimationListNode->m_contentList.size(); i++) {
		JSONItem* pAnimInfo = (JSONItem*) pAnimationListNode->m_contentList[i]->pValue;
		int* iTime = pAnimInfo->get<int*>("time");
		JSONItem* pPosition = pAnimInfo->get<JSONItem*>("position");
		char* szAsset = pAnimInfo->get<char*>("asset");
		char* szType = pAnimInfo->get<char*>("type");

		EAnimationEventType eType = EAnimationEventType_STEP;

		if (szType != NULL) {
			if (strcmp(szType, "step") == 0) {
				eType = EAnimationEventType_STEP;
			}
			else if (strcmp(szType, "linear") == 0) {
				eType = EAnimationEventType_LINEAR;
			}
		}

		IWidget* pWidget = NULL;

		if (szAsset != NULL) {
			for (int i = 0; i < pNewScene->getComponentCount(); i++) {
				IWidget* pW = pNewScene->getNComponent(i);
				if (strcmp(pW->getName(), szAsset) == 0) {
					pWidget = pW;
				}
			}
		}
		
		vect2df_t vPosition;
		if (pPosition) {
			int* x = pPosition->get<int*>("x");
			int* y = pPosition->get<int*>("y");
			if (x != NULL && y != NULL) {
				vPosition.x = *x;
				vPosition.y = *y;
			}
		}

		if (pWidget) {
			AnimationEvent* pNewAnimEvent = new AnimationEvent(*iTime, pWidget, vPosition, eType);
			pAnimationTimeline->addEvent(pNewAnimEvent);
		}
	}

	pSceneDescription->pScene = pNewScene;
	pSceneDescription->pAnimTimeline = pAnimationTimeline;

	return pNewScene;
}

IWidget* SceneManager::loadObject(Scene* pScene, char* szAsset, char* szName, char* szType) {
	IWidget* pNewObject = NULL;

	if (strcmp(szType, "sprite") == 0) {
		pNewObject = new Sprite(RscManager::get()->getImgRsc(szAsset), 0, 0);
	}
	else if (strcmp(szType, "animated_sprite") == 0) {
		pNewObject = new AnimatedSprite(RscManager::get()->getSprShtRsc(szAsset), 0, 0);
	}
	pNewObject->setName(szName);

	pScene->addComponent(pNewObject);
	return pNewObject;
}

SceneDescription* SceneManager::getScene(char* szName) {
	LLNode* pCurrNode = m_llSceneCollection.pHead;
	while (pCurrNode != NULL) {
		SceneDescription* pSceneDesc = (SceneDescription*) pCurrNode->pData;
		if (strcmp(pSceneDesc->pScene->getName(), szName) == 0) {
			return pSceneDesc;
		}
		pCurrNode = pCurrNode->pNext;
	}
	return NULL;
}

