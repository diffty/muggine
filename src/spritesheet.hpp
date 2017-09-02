//
//  spritesheet.hpp
//  Muggine
//
//  Created by DiFFtY on 02/09/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#ifndef spritesheet_hpp
#define spritesheet_hpp

#include <stdio.h>
#include "image.hpp"

class SpriteSheet : Image {
private:
    size2d_t m_gridSize;
    size2d_t m_frameSize;
    uint m_currFrame;
    uint m_length;
    
public:
    SpriteSheet(char* fileName, int gridWidth, int gridHeight, uint length);
    void draw(uint8* buffer, uint frame, int dstX, int dstY, bool reversed, bool masked);
    void draw(uint8* buffer, int dstX, int dstY, bool reversed, bool masked);
    void setFrame(uint newFrame);
    void nextFrame();
    void prevFrame();

};

#endif /* spritesheet_hpp */
