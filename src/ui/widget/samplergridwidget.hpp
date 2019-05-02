#ifndef SAMPLERGRIDWIDGET_HPP
#define SAMPLERGRIDWIDGET_HPP


#include <stdio.h>
#include "../core/common_types.hpp"
#include "../core/iwidget.hpp"
#include "../graphics/drawing.hpp"


class SamplerGridWidget : public IWidget {
private:
	unsigned int m_nbCellX;
	unsigned int m_nbCellY;
	unsigned int m_nbCells;
	unsigned char* m_aCellState;
	unsigned int* m_aPressedCells;
	unsigned int m_nbPressedCells;
	unsigned int* m_aPressedCellsCurrentLoop;
	unsigned int m_nbPressedCellsCurrentLoop;

	Color m_colorOn;
	Color m_colorPressed;
	Color m_colorOff;


public:
	SamplerGridWidget(int x, int y, int w, int h, int nbCellX, int nbCellY);
	~SamplerGridWidget();
	
	bool receiveTouchInput(vect2d_t touchPt);
	void update();
	void draw(uint8* fb);

	void onCellPress(vect2d_t gridCell);
	void onCellRelease(int cellId);
	bool isCellPressed(vect2d_t gridCell);
	bool isPointInSamplerGrid(vect2d_t pt);
	bool getCellInSamplerGridFromPoint(vect2d_t pt, vect2d_t* gridCell);
};

#endif