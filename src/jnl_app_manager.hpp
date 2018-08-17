//
//  jnl_app_manager.hpp
//  HyperDriftMayor
//
//  Created by DiFFtY on 13/05/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#ifndef jnl_app_manager_hpp
#define jnl_app_manager_hpp

#include <stdio.h>

#include "iwidget.hpp"
#include "scene.hpp"

#include "jnl_calendar_view.hpp"
#include "jnl_day_content_pane.hpp"


class JournalAppManager {
private:
    Scene* m_pCurrScene;

public:
    JournalAppManager();
    ~JournalAppManager();
    
    Scene* getScene();
    
    void update();
    void draw(uint8* buffer);
    
    void receiveTouchInput(vect2d_t inputPos);
};


#endif /* jnl_app_manager_hpp */
