//
//  editable_text.hpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef editable_text_hpp
#define editable_text_hpp

#include <stdio.h>
#include "iwidget.hpp"


class EditableText : public IWidget {
public:
    EditableText();
    
    void draw(uint8* buffer);
    void update();
    bool receiveTouchInput(vect2d_t vTouchPt);
};


#endif /* editable_text_hpp */
