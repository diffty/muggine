#ifndef BKO_GRID_HPP
#define BKO_GRID_HPP

#include "iwidget.hpp"
#include "bko_brick.hpp"
#include "linked_list.hpp"
#include "rsc_manager.hpp"


class Grid : public IWidget {
public:
	Grid(int x, int y, int nbBricksW, int nbBricksH, RscManager* pRscManager)
		: IWidget(x, y) {

		m_pRscManager = pRscManager;
		m_nbBricksW = nbBricksW;
		m_nbBricksH = nbBricksH;
            
        m_nbBricks = m_nbBricksW * m_nbBricksH;

		init();
	};

	~Grid();

	void init();
	void update();
	void draw(uint8* buffer);
    bool checkBrickAtPos(vect2d_t pos, uint* collidingBrickId);
    Brick* getBrickFromId(uint id);

private:
	// LinkedList m_brickList;
	Brick** m_brickList;
	size2d_t gridSize;
	RscManager* m_pRscManager;
	uint m_nbBricksW;
	uint m_nbBricksH;
    uint m_nbBricks;
};

#endif
