#ifndef RSC_MANAGER_HPP
#define RSC_MANAGER_HPP

#include <stdio.h>
#include <cstring>

#include "common_types.hpp"
#include "linked_list.hpp"
#include "image.hpp"
#include "spritesheet.hpp"
#include "font.hpp"
#include "platform_tools.hpp"
#include "jsonreader.hpp"



typedef struct Rsc {
    char* szName;
    char* szPath;
    void* pData;
} Rsc;


class RscManager {
private:
	LinkedList m_rscList;

	static RscManager* s_pInstance;

public:
	RscManager();
    ~RscManager();

	void loadFromJSON(const char* szRscJsonPath);
	bool loadImg(const char* szRscName, const char* szImgPath);
	bool loadSprSht(const char* szRscName, const char* szImgPath, int iGridWidth, int iGridHeight, uint uLength);
	bool loadFont(const char* szRscName, const char* szImgPath, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset);
	void unloadRsc(uint rscId);
	void freeAllRsc();
    LLNode* getRscNode(const char* szRscName);
	LLNode* getRscNode(uint rscId);
    Image* getImgRsc(const char* szRscName);
    Image* getImgRsc(uint rscId);
	SpriteSheet* getSprShtRsc(const char* szRscName);
	SpriteSheet* getSprShtRsc(uint rscId);
	Font* getFontRsc(const char* szRscName);
	Font* getFontRsc(uint rscId);
	uint getRscCount();
    
	static RscManager* get();
};

#endif
