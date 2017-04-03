#ifndef RSC_MANAGER_HPP
#define RSC_MANAGER_HPP

#include "common_types.hpp"
#include "linked_list.hpp"
#include "image.hpp"


class RscManager {
public:
	RscManager();
    ~RscManager();

	bool loadResource(char* rscPath);
	Image* getImgResource(uint rscId);
    void freeResources();

private:
	LinkedList m_rscList;
};

#endif
