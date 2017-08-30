#include "scene.hpp"


Scene::Scene() {
    initList(&m_contentList);
}

Scene::~Scene() {
    clear();
}

void Scene::addComponent(IWidget* widget) {
	LLNode* newNode = new LLNode;
	newNode->pData = widget;
	addNodeToList(&m_contentList, newNode);
}

void Scene::removeComponent(IWidget* widget) {
	LLNode* currNode = m_contentList.pHead;
	LLNode* prevNode = NULL;

	while (currNode != NULL) {
		if ((IWidget *) currNode->pData == widget) {
			printf("Removing component. %p\n", currNode);

			if (prevNode == NULL) {
				m_contentList.pHead = currNode->pNext;
				printf("New head is %p.\n", m_contentList.pHead);
			}
			else
				prevNode->pNext = currNode->pNext;

			if (currNode->pNext == NULL)
				m_contentList.pTail = prevNode;

			currNode->pNext = NULL;
            
            delete currNode;

			m_contentList.size--;
			return;
		}

		prevNode = currNode;
		currNode = currNode->pNext;
	}
}

IWidget* Scene::getNComponent(int n) {
	LLNode* currNode = m_contentList.pHead;
	int i = 0;

	while (currNode != NULL) {
		if (i == n) {
			return (IWidget*) currNode->pData;
		}

		currNode = currNode->pNext;
		i++;
	}

	return NULL;
}

IWidget* Scene::getFirstComponent() {
	if (m_contentList.size == 0) {
		return NULL;
	}

	return (IWidget*) m_contentList.pHead->pData;
}

unsigned int Scene::getComponentCount() {
	return m_contentList.size;
}

void Scene::receiveTouchInput(vect2d_t inputPos) {
	LLNode* currNode = m_contentList.pHead;

	while (currNode != NULL) {
		((IWidget*) currNode->pData)->receiveTouchInput(inputPos);
		currNode = currNode->pNext;
	}
}

void Scene::update() {
	LLNode* currNode = m_contentList.pHead;

	while (currNode != NULL) {
		((IWidget*) currNode->pData)->update();
		currNode = currNode->pNext;
	}
}

void Scene::draw(uint8* fb) {
	LLNode* currNode = m_contentList.pHead;

	while (currNode != NULL) {
		((IWidget*) currNode->pData)->draw(fb);
		currNode = currNode->pNext;
	}
}

void Scene::clear() {
    LLNode* currNode = m_contentList.pHead;
    LLNode* nextNode;
    
    while (currNode != NULL) {
        nextNode = currNode->pNext;
		delete currNode;
        currNode = nextNode;
    }
    
    m_contentList.pHead = NULL;
}
