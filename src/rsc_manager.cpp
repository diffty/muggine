#include "rsc_manager.hpp"


RscManager* RscManager::s_pInstance = NULL;


RscManager::RscManager() {
	RscManager::s_pInstance = this;
    initList(&m_rscList);
}

RscManager::~RscManager() {
	freeAllRsc();
}

bool RscManager::loadImg(char* imgPath) {
    Image* newImage = new Image(imgPath);

	if (newImage) {
		LLNode* newRscNode = new LLNode;
		newRscNode->pData = (void *) newImage;

		addNodeToList(&m_rscList, newRscNode);

		return true;
	}
	else {
		return false;
	}
}

bool RscManager::loadSprSht(char* szImgPath, int iGridWidth, int iGridHeight, uint uLength) {
	SpriteSheet* pNewSprSht = new SpriteSheet(szImgPath, iGridWidth, iGridHeight, uLength);

	if (pNewSprSht) {
		LLNode* newRscNode = new LLNode;
		newRscNode->pData = (void *)pNewSprSht;

		addNodeToList(&m_rscList, newRscNode);

		return true;
	}
	else {
		return false;
	}
}

bool RscManager::loadFont(char* szImgPath, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset) {
	Font* pNewFont = new Font(szImgPath, iGridWidth, iGridHeight, uLength, iSizeOffset);

	if (pNewFont) {
		LLNode* newRscNode = new LLNode;
		newRscNode->pData = (void *)pNewFont;

		addNodeToList(&m_rscList, newRscNode);

		return true;
	}
	else {
		return false;
	}
}

void RscManager::unloadRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);
	
	if (rscNode) {
		removeNodeFromList(&m_rscList, rscNode);
		Image* imgData = (Image*) rscNode->pData;
		delete imgData;
		delete rscNode;
	}
	else {
		printf("<!> Can't remove resource #%i: not found.\n", rscId);
	}
}

// TODO: changer le type d'id en un truc plus large
// TODO: faire une recherche par nom
Image* RscManager::getImgRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);

	if (rscNode) {
		return (Image*)rscNode->pData;
	}
	else {
		return NULL;
	}
}

// TODO: faire un truc qui fait que tu vas pas
// TODO: piocher à l'aveugle dans les ressources
SpriteSheet* RscManager::getSprShtRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);

	if (rscNode) {
		return (SpriteSheet*)rscNode->pData;
	}
	else {
		return NULL;
	}
}

Font* RscManager::getFontRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);

	if (rscNode) {
		return (Font*)rscNode->pData;
	}
	else {
		return NULL;
	}
}

LLNode* RscManager::getRscNode(uint rscId) {
	if (rscId >= m_rscList.size) {
		printf("<!> Invalid resource id: %d\n", rscId);
		return NULL;
	}

	uint i = 0;
	LLNode* currNode = m_rscList.pHead;

	while (i < rscId) {
		currNode = currNode->pNext;
		i++;
	}

	return currNode;
}

void RscManager::freeAllRsc() {
    LLNode* currNode = m_rscList.pHead;
    LLNode* nextNode;
    
    while (currNode != NULL) {
        nextNode = currNode->pNext;
        delete ((Image *) currNode->pData);
        delete currNode;
        currNode = nextNode;
    }
    
    m_rscList.pHead = NULL;
}

uint RscManager::getRscCount() {
	return m_rscList.size;
}

RscManager* RscManager::get() {
	if (s_pInstance == NULL) {
		return new RscManager;
	}
	return s_pInstance;
}