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
			(spriteSize.w + 2) * (i % m_nbBricksW),
			(spriteSize.h + 2) * (i / m_nbBricksW),
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
