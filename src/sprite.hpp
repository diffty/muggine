#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "iwidget.hpp"
#include "image.hpp"
#include "rsc_manager.hpp"


class Sprite : public IWidget {
protected:
	uint m_rscId;
	Image* m_pImg;
	SpriteSheet* m_pSprSht;
	uint m_uFrameNb;

public:
	Sprite(Image* pImg, vect2df_t vPos);
	Sprite(Image* pImg, float fXPos, float fYPos);
	Sprite(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos);
	Sprite(SpriteSheet* pSprSht, uint uFrameNb, float fXPos, float fYPos);
    
    void init(Image* pImg, float fXPos, float fYPos);
    void init(SpriteSheet* pSprSht, uint uFrameNb, float fXPos, float fYPos);
        
	uint getFrame();
	void setFrame(uint uNewFrame);

	void draw(uint8* buffer);
};

#endif
