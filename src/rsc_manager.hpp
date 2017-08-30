#ifndef RSC_MANAGER_HPP
#define RSC_MANAGER_HPP

#include "common_types.hpp"
#include "linked_list.hpp"
#include "image.hpp"


class RscManager {
public:
	RscManager();
    ~RscManager();

	bool loadRsc(char* rscPath);
	void unloadRsc(uint rscId);
	void freeAllRsc();
	LLNode* getRscNode(uint rscId);
	Image* getImgRsc(uint rscId);
	uint getRscCount();

private:
	LinkedList m_rscList;
};

#endif
