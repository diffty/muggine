#ifndef RSC_MANAGER_HPP
#define RSC_MANAGER_HPP

#include "common_types.hpp"
#include "linked_list.hpp"
#include "image.hpp"
#include "spritesheet.hpp"
#include "font.hpp"


class RscManager {
private:
	LinkedList m_rscList;

	static RscManager* s_pInstance;

public:
	RscManager();
    ~RscManager();

	bool loadImg(char* imgPath);
	bool loadSprSht(char* szImgPath, int iGridWidth, int iGridHeight, uint uLength);
	bool loadFont(char* szImgPath, int iGridWidth, int iGridHeight, uint uLength);
	void unloadRsc(uint rscId);
	void freeAllRsc();
	LLNode* getRscNode(uint rscId);
	Image* getImgRsc(uint rscId);
	SpriteSheet* getSprShtRsc(uint rscId);
	Font* getFontRsc(uint rscId);
	uint getRscCount();

	static RscManager* get();
};

#endif
