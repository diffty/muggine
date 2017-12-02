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
#include "sound.hpp"
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
	rscManager.loadSprSht("data/state_icons.bmp", 6, 2, 8);
	rscManager.loadImg("data/credits.bmp");
	rscManager.loadSprSht("data/ui3.bmp", 2, 1, 2);
	rscManager.loadSprSht("data/artist.bmp", 4, 2, 6);
#endif

	// Sound system
	Sound sound;

	sound.addSound("data/sound/intro-rix.mp3", true);
	sound.addSound("data/sound/music-menu.mp3", true);
	sound.addSound("data/sound/music-gameplay.mp3", true);
	sound.addSound("data/sound/sfx-clara.mp3", true);
	sound.addSound("data/sound/sfx-gain-cash.mp3", false);
	sound.addSound("data/sound/sfx-no-cash-no-game.mp3", false);
	sound.addSound("data/sound/sfx-object-buy-and-drop.mp3", false);
	sound.addSound("data/sound/sfx-object-taken.mp3", false);
	sound.addSound("data/sound/sfx-tlg-critical.mp3", true);
	sound.addSound("data/sound/sfx-tlg-disappear.mp3", false);
	sound.addSound("data/sound/sfx-tlg-is-busy.mp3", true);
	sound.addSound("data/sound/sfx-tlg-spawn.mp3", false);
	sound.addSound("data/sound/sfx-tlg-triggered.mp3", false);
	

	// TESTS
	/*Scene testScene;

	Sprite test(rscManager.getSprShtRsc(1), 3, { 0, 0 });

	Sprite test1(rscManager.getSprShtRsc(1), 12, { 0, 0 }); // sushis
	test1.setDrawOrder(100);
	Sprite test2(rscManager.getSprShtRsc(1), 13, { 4, 4 }); // plante
	test2.setDrawOrder(800);
	Sprite test3(rscManager.getSprShtRsc(1), 15, { 8, 8 }); // vinyl
	test3.setDrawOrder(50);
	Sprite test4(rscManager.getSprShtRsc(1), 16, { 12, 12 }); // pizza
	test4.setDrawOrder(500);

	test1.setParentWidget(&test);
	test2.setParentWidget(&test);
	test3.setParentWidget(&test);
	test4.setParentWidget(&test);

	testScene.addComponent(&test);

	printf("*** CHANGING DRAW ORDERZ ***\n");
	test1.setDrawOrder(1000);
	test2.setDrawOrder(800);
	test3.setDrawOrder(1200);
	test4.setDrawOrder(600);
	test2.setDrawOrder(1100);
	test2.setDrawOrder(1000);*/


	
	// Initing game manager
	TSGameManager gameManager;

	// Building scene
	Scene* gameScene = gameManager.getGameScene();
	Scene* menuScene = gameManager.getMenuScene();

	
	TSGameMode* gameMode = NULL;
	
	
	gfx->SetDoubleBuffering(false);

	uint8* fb = gfx->GetFramebuffer();

	sys->initLoop();

	// Main loop
	while (sys->mainLoop())
	{
		double deltaTime = sys->getDeltaTime();

		// printf("FPS: %u\n", (uint) (1./deltaTime));

		gfx->FillWithColor(0x00);

		gameManager.update();
		gameManager.draw(fb);

		/*if (sys->getInputSys()->IsKeyPressed(KEYB_A)) {
			test4.setDrawOrder(test4.getDrawOrder() + 1);
			printf("%i\n", test4.getDrawOrder());
		}
		if (sys->getInputSys()->IsKeyPressed(KEYB_Z)) {
			test4.setDrawOrder(test4.getDrawOrder() - 1);
			printf("%i\n", test4.getDrawOrder());
		}

		testScene.draw(fb);*/

		sound.update();

		// Flush and swap framebuffers
		gfx->FlushBuffer();
		gfx->SwapBuffer();

		// Wait for VBlank
		gfx->WaitForBlank();
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
