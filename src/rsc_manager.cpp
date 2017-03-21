#include "rsc_manager.hpp"


RscManager::RscManager() {
	initList(&m_rscList);
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
	}

	return (Image*) currNode->pData;
}