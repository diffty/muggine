#include "rsc_manager.hpp"


RscManager* RscManager::s_pInstance = NULL;


RscManager::RscManager() {
	RscManager::s_pInstance = this;
    initList(&m_rscList);
}

RscManager::~RscManager() {
	freeAllRsc();
}

void RscManager::loadFromJSON(const char* szRscJsonPath) {
	JSONReader json(szRscJsonPath);

	JSONItem* pStaticSpriteList = json.get<JSONItem*>("static_sprites");

	for (int i = 0; i < pStaticSpriteList->m_contentList.size(); i++) {
		JSONItem* assetInfoDict = pStaticSpriteList->get<JSONItem*>(i);
		char* szAssetName  = assetInfoDict->get<char*>("name");
		char* szPath       = assetInfoDict->get<char*>("path");
		loadImg(szAssetName, szPath);
	}

	JSONItem* pAnimatedSpriteList = json.get<JSONItem*>("animated_sprites");

	for (int i = 0; i < pAnimatedSpriteList->m_contentList.size(); i++) {
		JSONItem* assetInfoDict = pAnimatedSpriteList->get<JSONItem*>(i);
		char* szAssetName = assetInfoDict->get<char*>("name");
		char* szPath      = assetInfoDict->get<char*>("path");
		int* piGridWidth  = assetInfoDict->get<int*>("gridWidth");
		int* piGridHeight = assetInfoDict->get<int*>("gridHeight");
		int* piNbItems    = assetInfoDict->get<int*>("nbItems");
		loadSprSht(szAssetName, szPath, *piGridWidth, *piGridHeight, *piNbItems);
	}

	JSONItem* pFontList = json.get<JSONItem*>("fonts");

	for (int i = 0; i < pFontList->m_contentList.size(); i++) {
		JSONItem* assetInfoDict = pFontList->get<JSONItem*>(i);
		char* szAssetName = assetInfoDict->get<char*>("name");
		char* szPath      = assetInfoDict->get<char*>("path");
		int* piGridWidth  = assetInfoDict->get<int*>("gridWidth");
		int* piGridHeight = assetInfoDict->get<int*>("gridHeight");
		int* piNbItems    = assetInfoDict->get<int*>("nbItems");
		int* piSizeOffset = assetInfoDict->get<int*>("sizeOffset");
		loadFont(szAssetName, szPath, *piGridWidth, *piGridHeight, *piNbItems, *piSizeOffset);
	}
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
		strcpy(newRscData->szName, (char*)szRscName);
		newRscData->szPath = new char[strlen(szImgPath) + 1];
		strcpy(newRscData->szPath, (char*)szImgPath);
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
	FontBitmap* pNewFont = new FontBitmap(szConformedPath, iGridWidth, iGridHeight, uLength, iSizeOffset);
    delete szConformedPath;
    
	if (pNewFont) {
		LLNode* newRscNode = new LLNode;

        Rsc* newRscData = new Rsc;
        newRscData->szName =  new char[strlen(szRscName)+1];;
        strcpy(newRscData->szName, (char*)szRscName);
        newRscData->szPath = new char[strlen(szImgPath)+1];
        strcpy(newRscData->szPath, (char*)szImgPath);
        newRscData->pData = (void *) pNewFont;

		newRscNode->pData = (void *)newRscData;

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

Image* RscManager::getImgRsc(const char* szRscName) {
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

SpriteSheet* RscManager::getSprShtRsc(const char* szRscName) {
	LLNode* rscNode = getRscNode(szRscName);

	if (rscNode) {
		return (SpriteSheet*)((Rsc*)rscNode->pData)->pData;
	}
	else {
		return NULL;
	}
}

SpriteSheet* RscManager::getSprShtRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);

	if (rscNode) {
		return (SpriteSheet*)((Rsc*)rscNode->pData)->pData;
	}
	else {
		return NULL;
	}
}

FontBitmap* RscManager::getFontRsc(const char* szRscName) {
	LLNode* rscNode = getRscNode(szRscName);

	if (rscNode) {
		return (FontBitmap*)((Rsc*)rscNode->pData)->pData;
	}
	else {
		return NULL;
	}
}

FontBitmap* RscManager::getFontRsc(uint rscId) {
	LLNode* rscNode = getRscNode(rscId);

	if (rscNode) {
		return (FontBitmap*) ((Rsc*) rscNode->pData)->pData;
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

	printf("<!> Can't find resource named %s in loaded resources.\n", szRscName);
    return NULL;
}

LLNode* RscManager::getRscNode(uint rscId) {
	if (rscId >= m_rscList.size) {
		printf("<!> Can't find resource #%i in loaded resources.\n", rscId);
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
