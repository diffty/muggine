#include "bko_grid.hpp"

Grid::~Grid() {
	for (int i = 0; i < m_nbBricksW * m_nbBricksH; i++) {
		delete m_brickList[i];
	}

	delete[] m_brickList;
}

void Grid::init() {
	/*initList(&m_brickList);

	for (int i = 0; i < GRID_W * GRID_H; i++) {
		LLNode* newNode = new LLNode;
		newNode->pData = (void *) new Brick(0, m_pRscManager);
		addNodeToList(&m_brickList, newNode);
	}*/

	size2d_t spriteSize = m_pRscManager->getImgResource(0)->getSize();

	m_brickList = new Brick*[m_nbBricksW * m_nbBricksH];
	
	for (int i = 0; i < m_nbBricksW * m_nbBricksH; i++) {
		m_brickList[i] = new Brick(0, m_pRscManager);
		m_brickList[i]->translate(
			(spriteSize.w) * (i % m_nbBricksW),
			(spriteSize.h) * (i / m_nbBricksW),
			TRANSFORM_ABS);
	}
}

void Grid::update() {

}

void Grid::draw(uint8* buffer) {
	for (int i = 0; i < m_nbBricksW * m_nbBricksH; i++) {
		m_brickList[i]->draw(buffer);
	}
}

bool Grid::checkBrickAtPos(vect2d_t pos, uint* collidingBrickId) {
    for (int i = 0; i < m_nbBricks; i++) {
        Rect* currBrickRect = m_brickList[i]->getRect();
        
        if (m_brickList[i]->isActive() && currBrickRect->isPointInRect(pos.x, pos.y)) {
            *collidingBrickId = i;

            return true;
        }
    }
    return false;
}

Brick* Grid::getBrickFromId(uint id) {
    return m_brickList[id];
}

bool Grid::checkBrickBetweenPos(vect2d_t currBallCenter, vect2d_t nextBallCenter, size2d_t ballSize, uint* collidingBrickId) {
    vect2d_t deltaPos;
    deltaPos.x = nextBallCenter.x - currBallCenter.x;
    deltaPos.y = nextBallCenter.y - currBallCenter.y;
    
    double deltaDist = sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);
    
    for (int i = 0; i < (int) deltaDist; i++) {
        vect2d_t currPos;
        currPos.x = currBallCenter.x + (deltaPos.x / deltaDist) * i;
        currPos.y = currBallCenter.y + (deltaPos.y / deltaDist) * i;
        
        printf("checking: %ld, %ld\n", currPos.x, currPos.y);
        
        if (checkBrickAtPos(currPos, collidingBrickId)) {
            return true;
        }
    }
    
    return false;
}
