#include "rsc_manager.hpp"


RscManager::RscManager() {
    initList(&m_rscList);
}

RscManager::~RscManager() {
    freeResources();
}

bool RscManager::loadResource(char* rscPath) {
    Image* newImage = new Image(rscPath);

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

// TODO: changer le type d'id en un truc plus large
// TODO: faire une recherche par nom
Image* RscManager::getImgResource(uint rscId) {
	if (rscId >= m_rscList.size) {
		printf("<!> Invalid resource id: %d\n", rscId);
		return NULL;
	}

	uint8 i = 0;
	LLNode* currNode = m_rscList.pHead;

	while (i < rscId) {
		currNode = currNode->pNext;
		i++;
	}

	return (Image*) currNode->pData;
}

void RscManager::freeResources() {
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
