#include "scene.hpp"


Scene::Scene(char* szName) :
	IWidget(szName) {

	initList(&m_llHoverAwareWidgets);
	m_pRootWidget = this;
}

Scene::~Scene() {
	garbageCollect();
	clear();
}

void Scene::addComponent(IWidget* pWidget) {
	addChildWidget(pWidget);
}

void Scene::removeComponent(IWidget* pWidget) {
	removeChildWidget(pWidget);
}

IWidget* Scene::getNComponent(int n) {
	LLNode* pComponentNode = getNodeInList(&m_llChildrenWidgets, (uint)n);
	if (pComponentNode != NULL) {
		return (IWidget*) pComponentNode->pData;
	}
	else {
		return NULL;
	}
}

IWidget* Scene::getFirstComponent() {
	if (m_llChildrenWidgets.size == 0) {
		return NULL;
	}

	return (IWidget*) getNodeInList(&m_llChildrenWidgets, 0);
}

unsigned int Scene::getComponentCount() {
	return m_llChildrenWidgets.size;
}

void Scene::update() {
	updateChildren();
}

void Scene::draw(uint8* pBuffer) {
	drawChildren(pBuffer);
}

void Scene::clear() {
	LLNode* currNode = m_llChildrenWidgets.pHead;
    LLNode* nextNode;
    
    while (currNode != NULL) {
        nextNode = currNode->pNext;
		delete currNode;
        currNode = nextNode;
    }
    
	m_llChildrenWidgets.pHead = NULL;

	garbageCollect();
}

void Scene::destroy() {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		IWidget* currWidget = ((IWidget*)currNode->pData);
		delete currWidget;
		currNode = currNode->pNext;
	}

	m_llChildrenWidgets.pHead = NULL;
	m_llChildrenWidgets.pTail = NULL;

	garbageCollect();
}
