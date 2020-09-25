#ifndef PAGINATED_WIDGET_LAYOUT_HPP
#define PAGINATED_WIDGET_LAYOUT_HPP

#include "../../core/iwidget.hpp"
#include "../../utils/linked_list.hpp"


class PaginatedWidgetLayout : public IWidget {
protected:
	LinkedList m_llWidgetList;
	IWidget* m_pCurrWidget;
	int m_pCurrWidgetId;

public:
	PaginatedWidgetLayout(vect2df_t vPos, size2df_t sSize);
	~PaginatedWidgetLayout();

	void addWidget(IWidget* pWidget);
	void switchWidget(uint uNum);
	void switchToPrevWidget();
	void switchToNextWidget();

	void update();
	void draw(drawbuffer* pBuffer);
	bool receiveTouchInput(vect2d_t inputPos);

	void updateChildren();

	void destroyAllWidgets();
};

#endif
