#include "rsc_manager.hpp"

RscManager* RscManager::s_pInstance = NULL;


RscManager::RscManager() {
	RscManager::s_pInstance = this;
    initList(&m_rscList);
}

RscManager::~RscManager() {
	freeAllRsc();
}

bool RscManager::loadImg(const char* szRscName, const char* szImgPath) {
    char* szConformedPath = platformConformPath(szImgPath);
    Image* newImage = new Image(szConformedPath);
    delete szConformedPath;

	if (newImage) {
		LLNode* newRscNode = new LLNode;
        
        Rsc* newRscData = new Rsc;
        
        newRscData->szName = new char[strlen(szRscName)+1];
        strcpy(newRscData->szName, szRscName);
        newRscData->szPath = new char[strlen(szImgPath)+1];
        strcpy(newRscData->szPath, szImgPath);
        newRscData->pData = (void *) newImage;
        
		newRscNode->pData = (void *) newRscData;

		addNodeToList(&m_rscList, newRscNode);

		return true;
	}
	else {
		return false;
	}
}

bool RscManager::loadSprSht(const char* szRscName, const char* szImgPath, int iGridWidth, int iGridHeight, uint uLength) {
    char* szConformedPath = platformConformPath(szImgPath);
	SpriteSheet* pNewSprSht = new SpriteSheet(szConformedPath, iGridWidth, iGridHeight, uLength);
    delete szConformedPath;
    
	if (pNewSprSht) {
		LLNode* newRscNode = new LLNode;
        
        Rsc* newRscData = new Rsc;
        newRscData->szName =  new char[strlen(szRscName)+1];;
        strcpy((char*) szRscName, newRscData->szName);
        newRscData->szPath = new char[strlen(szImgPath)+1];
        strcpy((char*) szImgPath, newRscData->szPath);
        newRscData->pData = (void *) pNewSprSht;
        
        newRscNode->pData = (void *) newRscData;


		addNodeToList(&m_rscList, newRscNode);

		return true;
	}
	else {
		return false;
	}
}

bool RscManager::loadFont(const char* szRscName, const char* szImgPath, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset) {
    char* szConformedPath = platformConformPath(szImgPath);
	Font* pNewFont = new Font(szConformedPath, iGridWidth, iGridHeight, uLength, iSizeOffset);
    delete szConformedPath;
    
	if (pNewFont) {
		LLNode* newRscNode = new LLNode;

        Rsc* newRscData = new Rsc;
        newRscData->szName =  new char[strlen(szRscName)+1];;
        strcpy((char*) szRscName, newRscData->szName);
        newRscData->szPath = new char[strlen(szImgPath)+1];
        strcpy((char*) szImgPath, newRscData->szPath);
        newRscData->pData = (void *) pNewFont;

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

Image* RscManager::getImgRsc(char* szRscName) {
    LLNode* rscNode = getRscNode(szRscName);
    
    if (rscNode) {
        return (Image*) ((Rsc*) rscNode->pData)->pData;
    }
    else {
        return NULL;
    }
}

Image* RscManager::getImgRsc(uint rscId) {
    LLNode* rscNode = getRscNode(rscId);
    
    if (rscNode) {
        return (Image*) ((Rsc*) rscNode->pData)->pData;
    }
    else {
        return NULL;
    }
}

// TODO: faire un truc qui fait que tu vas pas
// TODO: piocher a l'aveugle dans les ressources
SpriteSheet* RscManager::getSprShtRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);

	if (rscNode) {
		return (SpriteSheet*) ((Rsc*) rscNode->pData)->pData;
	}
	else {
		return NULL;
	}
}

Font* RscManager::getFontRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);

	if (rscNode) {
		return (Font*) ((Rsc*) rscNode->pData)->pData;
	}
	else {
		return NULL;
	}
}

LLNode* RscManager::getRscNode(const char* szRscName) {
    LLNode* currNode = m_rscList.pHead;
    
    while (currNode != NULL) {
        Rsc* pRscNode = (Rsc*) currNode->pData;
        if (pRscNode) {
            if (strcmp(pRscNode->szName, szRscName) == 0) {
                return currNode;
            }
        }
        currNode = currNode->pNext;
    }
    return NULL;
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
        Rsc* pRscNode = (Rsc*) currNode->pData;
        
        delete ((Image *) pRscNode->pData);
        delete pRscNode;
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
