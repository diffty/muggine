#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_SDL
#include <SDL2/SDL.h>
#endif

#include "linked_list.hpp"
#include "common_types.hpp"
#include "scene.hpp"
#include "system.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "rsc_manager.hpp"
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "fsm.hpp"
#include "font.hpp"
#include "text.hpp"
#include "ts_main_character.hpp"
#include "ts_game_manager.hpp"
#include "ts_things_manager.hpp"
#include "ts_things_store.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_critical_thing.hpp"
#include "ts_workguy_thing.hpp"
#include "ts_winning_thing.hpp"
#include "ts_game_mode.hpp"
#include "ts_main_menu.hpp"
#include "image_button_widget.hpp"
#include "ts_clone_machine.hpp"
#include "ts_text_bubble.hpp"

#include <time.h>

#ifdef TARGET_WIN
#include <windows.h>
#endif


int intFromStr(char* szStr) {
	int i = 0;
	int iStrLen = strlen(szStr);
	int iDigitNum = 0;
	int iRes = 0;

	char c;

	while ((c = szStr[i]) != '\0') {
		iDigitNum = (iStrLen - 1) - i;
		iRes += atoi(&c) * pow(10, iDigitNum);

		i++;
	}

	return iRes;
}

void MainApp(System* sys, Graphics* gfx) {
#ifdef TARGET_3DS
	Result rc = romfsInit();
#endif
    
	RscManager rscManager;

#ifdef TARGET_3DS
	rscManager.loadImg("romfs:/data/bg1.bmp");   // Catch le crash quand le path est pas bon stp
	rscManager.loadImg("romfs:/data/bg2.bmp");
	rscManager.loadImg("romfs:/data/bg3.bmp");
	rscManager.loadImg("romfs:/data/bg4.bmp");

#else
    rscManager.loadSprSht("data/tlg_animated.bmp", 12, 17, 202);
	rscManager.loadSprSht("data/objects.bmp", 5, 7, 31);
	rscManager.loadImg("data/room.bmp");
	rscManager.loadSprSht("data/characters.bmp", 1, 1, 1);
	rscManager.loadFont("data/font-big.bmp", 16, 16, 256, -1);
	rscManager.loadFont("data/font-small.bmp", 16, 16, 256, -1);
	rscManager.loadSprSht("data/ui.bmp", 4, 1, 4);
	rscManager.loadSprSht("data/ui2.bmp", 1, 1, 1);
	rscManager.loadSprSht("data/clara.bmp", 6, 4, 22);
	rscManager.loadSprSht("data/clone_machine.bmp", 1, 1, 1);
	rscManager.loadImg("data/title.bmp");
	rscManager.loadSprSht("data/main_menu_ui.bmp", 6, 1, 6);
	rscManager.loadFont("data/font-small-black.bmp", 16, 16, 256, -1);
#endif

	TSGameManager gameManager;

	// Building scene
	Scene* gameScene = gameManager.getGameScene();
	Scene* menuScene = gameManager.getMenuScene();

	TSGameMode* gameMode = NULL;
	
	gfx->SetDoubleBuffering(false);

	uint8* fb = gfx->GetFramebuffer();

	sys->initLoop();

	int i = 0;

	// Main loop
	while (sys->mainLoop())
	{
		double deltaTime = sys->getDeltaTime();

		// printf("FPS: %u\n", (uint) (1./deltaTime));

		gfx->FillWithColor(0x00);

		gameManager.update();
		gameManager.draw(fb);

		// Flush and swap framebuffers
		gfx->FlushBuffer();
		gfx->SwapBuffer();

		// Wait for VBlank
		gfx->WaitForBlank();

		i++;
	}

	rscManager.freeAllRsc();

	// Exit services
	gfx->Exit();
}

int main(int argc, char **argv)
{
	System* sys = System::get();
	Graphics gfx(sys);

#ifdef TARGET_SDL
	sys->initWindow();
#endif

	gfx.Init();
	sys->consoleInit();  // toujours initialiser la console après l'init de Gfx, surtout pour la 3DS.

	MainApp(sys, &gfx);

	delete sys;

	return 0;
}
