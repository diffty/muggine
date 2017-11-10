#ifndef GRID_WIDGET_LAYOUT_HPP
#define GRID_WIDGET_LAYOUT_HPP

#include "common_types.hpp"
#include "iwidget.hpp"
#include "linked_list.hpp"


class GridWidgetLayout : public IWidget
{
protected:
	LinkedList m_llContentList;

	uint m_uNbCellX;
	uint m_uNbCellY;

public:
	GridWidgetLayout(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY);
	~GridWidgetLayout();

	void addWidget(IWidget* pWidget);
	void moveWidgetToGrid(uint uWidgetId);
	void moveWidgetToGrid(IWidget* pWidget);
	void moveWidgetToGrid(IWidget* pWidget, uint gridPos);

	int getWidgetIdInLayout(IWidget* pWidget);

	void update();
	void draw(uint8* fb);
	void receiveTouchInput(vect2d_t inputPos);

	void updateChildren();

	void destroyAllWidgets();
};


#endif