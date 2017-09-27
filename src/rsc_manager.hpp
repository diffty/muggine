#ifndef RSC_MANAGER_HPP
#define RSC_MANAGER_HPP

#include "common_types.hpp"
#include "linked_list.hpp"
#include "image.hpp"
#include "spritesheet.hpp"


class RscManager {
public:
	RscManager();
    ~RscManager();

	bool loadImg(char* imgPath);
	bool loadSprSht(char* imgPath, int gridWidth, int gridHeight, uint length);
	void unloadRsc(uint rscId);
	void freeAllRsc();
	LLNode* getRscNode(uint rscId);
	Image* getImgRsc(uint rscId);
	SpriteSheet* getSprShtRsc(uint rscId);
	uint getRscCount();

private:
	LinkedList m_rscList;
};

#endif
