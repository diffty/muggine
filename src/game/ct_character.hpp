//
//  ct_character.hpp
//  DeMuGGinE
//
//  Created by DiFFtY on 16/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#ifndef ct_character_hpp
#define ct_character_hpp

#include <stdio.h>
#include "../core/idrawable.hpp"
#include "../graphics/drawbuffer.hpp"
#include "../graphics/sprite.hpp"
#include "../core/iwidget.hpp"


class CTCharacter: public IWidget {
public:
    CTCharacter();
    ~CTCharacter();
    
    DrawBuffer* getDrawBuffer();
    
    void draw(drawbuffer* pBuffer);
    
private:
    DrawBuffer* m_pDrawBuffer;
};

#endif /* ct_character_hpp */
