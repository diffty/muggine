#include "or_gameover_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"
#include "or_game_mode.hpp"


ORGameOverScreen::ORGameOverScreen() {
    if (ORGameMode::get()->getPopularityValue() >= 0.) {
        m_pEndImgSpr = new Sprite(RscManager::get()->getImgRsc(31), {0, 0});
    }
    else {
        m_pEndImgSpr = new Sprite(RscManager::get()->getImgRsc(32), {0, 0});
    }
    
    m_pEndImgSpr->translate(
                           (SCREEN_WIDTH/2) - (m_pEndImgSpr->getRect()->getSize().w / 2),
                           (SCREEN_HEIGHT/2) - (m_pEndImgSpr->getRect()->getSize().h / 2),
                           TRANSFORM_ABS);
    
    addChildWidget(m_pEndImgSpr);
}

void ORGameOverScreen::update() {
	updateChildren();
}


void ORGameOverScreen::draw(uint8* buffer) {
    drawChildren(buffer);
}

