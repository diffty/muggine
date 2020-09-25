#include "xx_gameover_screen.hpp"

#include "../manager/rsc_manager.hpp"
#include "../core/system.hpp"
#include "../game/game_mode.hpp"


XXGameOverScreen::XXGameOverScreen() {
    // Stuff that's happening on game over (displaying a splash or idk..)
}

void XXGameOverScreen::update() {
    updateChildren();
}


void XXGameOverScreen::draw(drawbuffer* pBuffer) {
    drawChildren(pBuffer);
}
