#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "iwidget.hpp"
#include "image.hpp"
#include "rsc_manager.hpp"

enum ETransformMode {
	TRANSFORM_REL,
	TRANSFORM_ABS,
};


class Sprite : public IWidget {
public:
	Sprite(uint rscId, RscManager* rscManager, int x = 0, int y = 0);
	void draw(uint8* buffer);
	void translate(int x, int y, ETransformMode transformMode = TRANSFORM_REL);

	void setActive(bool bIsActive);
    bool isActive();

private:
	uint m_rscId;
	Image* m_image;
	bool m_bIsActive = true;
};

#endif
