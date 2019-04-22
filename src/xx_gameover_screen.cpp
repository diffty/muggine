#include "xx_gameover_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"
#include "game_mode.hpp"


XXGameOverScreen::XXGameOverScreen() {
    // Stuff that's happening on game over (displaying a splash or idk..)
}

void XXGameOverScreen::update() {
    updateChildren();
}


void XXGameOverScreen::draw(uint8* buffer) {
    drawChildren(buffer);
}
