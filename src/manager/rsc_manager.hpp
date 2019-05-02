#ifndef RSC_MANAGER_HPP
#define RSC_MANAGER_HPP

#include <stdio.h>
#include <cstring>

#include "../core/common_types.hpp"
#include "../utils/linked_list.hpp"
#include "../graphics/image.hpp"
#include "../graphics/spritesheet.hpp"
#include "../graphics/font_bmp.hpp"
#include "../core/platform_tools.hpp"
#include "../utils/jsonreader.hpp"



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
	FontBitmap* getFontRsc(const char* szRscName);
	FontBitmap* getFontRsc(uint rscId);
	uint getRscCount();
    
	static RscManager* get();
};

#endif
