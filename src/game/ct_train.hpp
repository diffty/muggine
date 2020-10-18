//
//  ct_train.hpp
//  DeMuGGinE
//
//  Created by DiFFtY on 25/09/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#ifndef ct_train_hpp
#define ct_train_hpp

#include <stdio.h>
#include "../graphics/image.hpp"
#include "../utils/linked_list.hpp"
#include "../core/iwidget.hpp"
#include "../graphics/animatedsprite.hpp"
#include "../graphics/scene.hpp"
#include "../game/ct_door.hpp"


class CTTrain : public IWidget {
public:
    CTTrain();
    ~CTTrain();
    
    void addTransition();
    void addWagon();
    void addPart(IWidget* pNewPart);
    CTDoor* addDoorForPart(IWidget* pTrainPart);
    Sprite* addWallForPart(IWidget* pTrainPart);
        
    Scene* getWallScene();
    
    CTDoor* isInFrontOfDoor();
    
    void recomputeRect();
        
    void update();
    void draw(drawbuffer* pBuffer);
        
private:
    Scene* m_pTrainScene = NULL;
    Scene* m_pWallScene = NULL;
    
    AnimatedSprite* m_pCha;
    AnimatedSprite* m_pFred;

    Image* m_pTransitionWagonImg;
    Image* m_pWagonImg;
    LinkedList m_llWagonList;
    
    float m_fPauseTime;
    float m_fSpeed = 20.;

};

#endif /* ct_train_hpp */
