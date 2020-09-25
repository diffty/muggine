#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "../core/iwidget.hpp"
#include "image.hpp"
#include "../manager/rsc_manager.hpp"
#include "drawbuffer.hpp"


class Sprite : public IWidget {
protected:
	uint m_rscId;
    DrawBuffer* m_pDrawBuf;
	Image* m_pImg;
	SpriteSheet* m_pSprSht;
	uint m_uFrameNb;

public:
    Sprite(DrawBuffer* pDrawBuf, vect2df_t vPos, char* szName = NULL);
    Sprite(DrawBuffer* pDrawBuf, float fXPos, float fYPos, char* szName = NULL);
    Sprite(Image* pImg, vect2df_t vPos, char* szName = NULL);
    Sprite(Image* pImg, float fXPos, float fYPos, char* szName = NULL);
	Sprite(SpriteSheet* pSprSht, uint uFrameNb, vect2df_t vPos, char* szName = NULL);
	Sprite(SpriteSheet* pSprSht, uint uFrameNb, float fXPos, float fYPos, char* szName = NULL);
    
    void init(DrawBuffer* pDrawBuf, float fXPos, float fYPos);
    void init(Image* pImg, float fXPos, float fYPos);
    void init(SpriteSheet* pSprSht, uint uFrameNb, float fXPos, float fYPos);
        
	uint getFrame();
	void setFrame(uint uNewFrame);

	void draw(drawbuffer* pBuffer);
};

#endif
