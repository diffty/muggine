#ifndef SCENE_HPP
#define SCENE_HPP


#include <stdio.h>
#include <cstdlib>
#include <3ds.h>

#include "linked_list.hpp"
#include "widget.hpp"


class Scene {
private:
	LinkedList m_contentList;

public:
	Scene();
	
	void addComponent(IWidget* w);
	void removeComponent(IWidget* widget);
	IWidget* getFirstComponent();
	IWidget* getNComponent(int n);

	void receiveTouchInput(vect2d_t inputPos);
	void update();
	void draw(u8* fb);
};


#endif