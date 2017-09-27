#ifndef PAGINATED_WIDGET_LAYOUT_HPP
#define PAGINATED_WIDGET_LAYOUT_HPP

#include "common_types.hpp"
#include "iwidget.hpp"
#include "linked_list.hpp"


class PaginatedWidgetLayout : public IWidget
{
public:
	PaginatedWidgetLayout(vect2df_t vPos, size2df_t sSize, uint uNbCellX, uint uNbCellY);
	~PaginatedWidgetLayout();

	void addWidget(IWidget* pWidget);
	void moveWidgetToGrid(uint uWidgetId);

	void update();
	void draw(uint8* fb);
	void receiveTouchInput(vect2d_t inputPos);

	void updateChildren();


protected:
	LinkedList m_llContentList;

	uint m_uNbCellX;
	uint m_uNbCellY;
};


#endif