#ifndef SCENE_HPP
#define SCENE_HPP


#include <stdio.h>
#include <cstdlib>

#ifdef TARGET_3DS
#include <3ds.h>
#endif

#include "linked_list.hpp"
#include "iwidget.hpp"


class Scene : public IWidget {
private:
	LinkedList m_llHoverAwareWidgets;

public:
	Scene();
    ~Scene();
	
	void addComponent(IWidget* w);
	void removeComponent(IWidget* widget);
	IWidget* getFirstComponent();
	IWidget* getNComponent(int n);
	unsigned int getComponentCount();

	//void receiveTouchInput(vect2d_t inputPos);
	void update();
	void draw(uint8* fb);
    void clear();

	void destroy();

	void test() { printf("LOLPIPI\n"); };
};


#endif
