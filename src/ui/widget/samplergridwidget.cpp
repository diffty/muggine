#include "samplergridwidget.hpp"


SamplerGridWidget::SamplerGridWidget(int x, int y, int w, int h, int nbCellX, int nbCellY)
	: 
	  IWidget(x, y, w, h),
	  m_colorOn(255, 0, 3),
	  m_colorOff(3, 0, 255),
	  m_colorPressed(3, 255, 3)
{

	m_nbCellX = nbCellX;
	m_nbCellY = nbCellY;
	m_nbCells = m_nbCellX * m_nbCellY;
	m_aCellState = new unsigned char[m_nbCells];
	m_aPressedCells = new unsigned int[m_nbCells];
	m_aPressedCellsCurrentLoop = new unsigned int[m_nbCells];
	m_nbPressedCells = 0;
	m_nbPressedCellsCurrentLoop = 0;

	for (int i = 0; i < m_nbCells; i++) {
		m_aCellState[i] = 0;
	}
}

SamplerGridWidget::~SamplerGridWidget() {
	delete m_aCellState;
	delete m_aPressedCells;
	delete m_aPressedCellsCurrentLoop;
}

void SamplerGridWidget::update() {
	updateChildren();

	for (int i = m_nbPressedCells - 1; i >= 0; i--) {
		bool bIsCellStillPressed = false;
		
		for (int j = 0; j < m_nbPressedCellsCurrentLoop; j++) {
			if (m_aPressedCells[i] == m_aPressedCellsCurrentLoop[j]) {
				bIsCellStillPressed = true;
			}
		}

		if (bIsCellStillPressed == false) {
			int cellId = m_aPressedCells[i];
			printf("cell %d released\n", cellId);
			if (m_aCellState[cellId] == 2) {
				m_aCellState[cellId] = 1;
			}
			else if (m_aCellState[cellId] == 4) {
				m_aCellState[cellId] = 0;
			}

			onCellRelease(cellId);
		}
	}

	m_nbPressedCellsCurrentLoop = 0;
}

void SamplerGridWidget::draw(drawbuffer* pBuffer) {
	if (m_bIsActive) {
		drawChildren(pBuffer);

		int cellW = m_rect.getSize().w / m_nbCellX;
		int cellH = m_rect.getSize().h / m_nbCellY;

		for (int y = 0; y < m_nbCellY; y++) {
			for (int x = 0; x < m_nbCellX; x++) {
				if (m_aCellState[y * m_nbCellX + x] == 1) {
					drawBox(pBuffer,
						x + cellW * x,
						y + cellH * y,
						x + cellW * (x + 1) - 2,
						y + cellH * (y + 1) - 1,
						&m_colorOn
					);
				}
				else if (m_aCellState[y * m_nbCellX + x] == 2 || m_aCellState[y * m_nbCellX + x] == 4) {
					drawBox(pBuffer,
						x + cellW * x,
						y + cellH * y,
						x + cellW * (x + 1) - 2,
						y + cellH * (y + 1) - 1,
						&m_colorPressed
					);
				}
				else {
					drawBox(pBuffer,
						x + cellW * x,
						y + cellH * y,
						x + cellW * (x + 1) - 2,
						y + cellH * (y + 1) - 1,
						&m_colorOff
					);
				}
			}
		}
	}
}

bool SamplerGridWidget::receiveTouchInput(vect2d_t touchPt) {
	vect2d_t inGridTouchPt;
	
	if (getCellInSamplerGridFromPoint(touchPt, &inGridTouchPt)) {
		int cellId = inGridTouchPt.x + inGridTouchPt.y * m_nbCellX;

		if (!isCellPressed(inGridTouchPt)) {
			printf("cell %d pressed\n", cellId);
			onCellPress(inGridTouchPt);
		}

		m_aPressedCellsCurrentLoop[m_nbPressedCellsCurrentLoop] = cellId;
		m_nbPressedCellsCurrentLoop++;
		return true;
	}

	return false;
}

void SamplerGridWidget::onCellPress(vect2d_t gridCell) {
	unsigned int cellId = gridCell.x + gridCell.y * m_nbCellX;

	if (m_aCellState[cellId] == 2 || m_aCellState[cellId] == 4) {
		return;
	}

	m_aCellState[cellId] = (m_aCellState[cellId] == 0) ? 2 : 4;
	m_aPressedCells[m_nbPressedCells] = cellId;
	m_nbPressedCells++;
}

void SamplerGridWidget::onCellRelease(int cellId) {
	int i;

	for (i = 0; i < m_nbPressedCells; i++) {
		if (m_aPressedCells[i] == cellId) break;
	}

	for (i = i+1; i < m_nbPressedCells; i++) {
		m_aPressedCells[i-1] = m_aPressedCells[i];
	}

	m_aPressedCells[i-1] = -1;
	m_nbPressedCells--;
}

bool SamplerGridWidget::isCellPressed(vect2d_t gridCell) {
	int i;

	unsigned int cellId = gridCell.x + gridCell.y * m_nbCellX;

	for (i = 0; i < m_nbPressedCells; i++) {
		if (m_aPressedCells[i] == cellId) {
			return true;
		}
	}

	return false;
}

bool SamplerGridWidget::isPointInSamplerGrid(vect2d_t pt) {
	if ((m_rect.getPos().x <= pt.x && m_rect.getPos().x + m_rect.getSize().w >= pt.x)
		|| (m_rect.getPos().y <= pt.y && m_rect.getPos().y + m_rect.getSize().h >= pt.y))
		return true;
	else
		return false;
}

bool SamplerGridWidget::getCellInSamplerGridFromPoint(vect2d_t pt, vect2d_t* gridCell) {
	gridCell->x = (int) (((float) (pt.x - m_rect.getPos().x) / m_rect.getSize().w) * m_nbCellX);
	gridCell->y = (int) (((float) (pt.y - m_rect.getPos().y) / m_rect.getSize().h) * m_nbCellY);

	if (gridCell->x < 0 || gridCell->y < 0 || gridCell->x >= m_nbCellX || gridCell->y >= m_nbCellY)
		return false;
	else
		return true;
}
