#include "scene.hpp"


Scene::Scene() {
	initList(&m_llHoverAwareWidgets);
	m_pRootWidget = this;
}

Scene::~Scene() {
    clear();
}

void Scene::addComponent(IWidget* pWidget) {
	//pWidget->setRootWidget(this);
	//pWidget->updateChildren();
	addChildWidget(pWidget);
}

void Scene::removeComponent(IWidget* pWidget) {
	removeChildWidget(pWidget);
}

IWidget* Scene::getNComponent(int n) {
	return (IWidget*) getNodeInList(&m_llChildrenWidgets, (uint) n);
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


void Scene::receiveTouchInput(vect2d_t inputPos) {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		((IWidget*) currNode->pData)->receiveTouchInput(inputPos);
		currNode = currNode->pNext;
	}
}

void Scene::update() {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = (IWidget*) currNode->pData;
		pCurrWidget->update();

		currNode = currNode->pNext;
	}

	garbageCollect();
}

void Scene::draw(uint8* fb) {
	LLNode* currNode = m_llChildrenWidgets.pHead;

	while (currNode != NULL) {
		IWidget* pCurrWidget = (IWidget*)currNode->pData;

		pCurrWidget->draw(fb);
		currNode = currNode->pNext;
	}
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
}

void Scene::destroy() {
	LLNode* currNode = m_llChildrenWidgets.pHead;
	LLNode* nextNode;

	while (currNode != NULL) {
		nextNode = currNode->pNext;
		IWidget* currWidget = ((IWidget*)currNode->pData);
		delete currWidget;
		delete currNode;
		currNode = nextNode;
	}

	m_llChildrenWidgets.pHead = NULL;
}
