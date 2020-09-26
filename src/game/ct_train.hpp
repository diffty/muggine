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


class CTTrain : public IWidget {
public:
    CTTrain();
    ~CTTrain();
    
    void addTransition();
    void addWagon();
    void destroyWagon();
    
    void update();
    void draw(drawbuffer* pBuffer);
        
private:
    Image* m_pTransitionWagonImg;
    Image* m_pWagonImg;
    LinkedList m_llWagonList;
};

#endif /* ct_train_hpp */
