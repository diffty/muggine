#ifndef GRID_WIDGET_LAYOUT_HPP
#define GRID_WIDGET_LAYOUT_HPP

#include "../../core/common_types.hpp"
#include "../../core/iwidget.hpp"
#include "../../utils/linked_list.hpp"


class GridWidgetLayout : public IWidget
{
protected:
	uint m_uNbCellX;
	uint m_uNbCellY;

	int m_iGridOffsetX;

public:
	GridWidgetLayout(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY);
	~GridWidgetLayout();

	void addWidget(IWidget* pWidget);
	void moveWidgetToGrid(uint uWidgetId);
	void moveWidgetToGrid(IWidget* pWidget);
	void moveWidgetToGrid(IWidget* pWidget, uint gridPos);

	vect2df_t getPosInGrid(IWidget* pWidget, uint uGridPos);
	vect2df_t getNextPosInGrid(IWidget* pWidget);

	int getWidgetIdInLayout(IWidget* pWidget);

	void setGridOffsetX(int x);

	void update();
	void draw(uint8* buffer);
	//void receiveTouchInput(vect2d_t inputPos);

	void destroyAllWidgets();
};


#endif