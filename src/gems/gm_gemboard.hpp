//
//  gm_gemboard.hpp
//  DeMuGGinE
//
//  Created by DiFFtY on 06/10/2020.
//  Copyright © 2020 RIX. All rights reserved.
//

#ifndef gm_gemboard_hpp
#define gm_gemboard_hpp

#include <stdio.h>

#include "../core/common_types.hpp"
#include "../core/iwidget.hpp"
#include "../gems/gm_gem.hpp"
#include "../utils/linked_list.hpp"


class GemBoard : public IWidget {
public:
    GemBoard(int iGridW, int iGridH, SpriteSheet* pGemSprSht);
    ~GemBoard();
    
    void generateGrid();
    void generateNewGems(int rowId, int nbGemsToGenerate);
    void destroyGem(Gem* pGem);
    void destroyGem(int iGridX, int iGridY);
    void destroyAllGemsOfColor(int iColorId);

    void purgeGemTrash();
    
    vect2d_t findGemGridCoord(Gem* pGem);

    void onGemDestruction(vect2d_t vGemGridCoord);

    void draw(drawbuffer* pBuffer);
    void update();
    bool receiveTouchInput(vect2d_t touchPt);
    
private:
    Gem** m_gemGrid;
    Gem** m_gemReserveGrid;
    
    int m_iGridW = 1;
    int m_iGridH = 1;
    
    LinkedList m_llGemDestroyList;
    SpriteSheet* m_pGemSprSht;
};

#endif /* gm_gemboard_hpp */
