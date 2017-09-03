#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "iwidget.hpp"
#include "image.hpp"
#include "rsc_manager.hpp"


class Sprite : public IWidget {
public:
	Sprite(uint rscId, RscManager* rscManager, int x = 0, int y = 0);
	void draw(uint8* buffer);

private:
	uint m_rscId;
	Image* m_image;
};

#endif
