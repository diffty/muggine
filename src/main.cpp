#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_SDL
#include <SDL2/SDL.h>
#endif

#include "samplergridwidget.hpp"
#include "linked_list.hpp"
#include "common_types.hpp"
#include "scene.hpp"
#include "buttonwidget.hpp"
#include "system.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "rsc_manager.hpp"
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "parallax_background.hpp"
#include "fsm.hpp"
#include "font.hpp"
#include "text.hpp"
#include "ts_main_character.hpp"
#include "ts_interactable_thing.hpp"
#include "ts_things_manager.hpp"
#include "ts_things_store.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_critical_thing.hpp"
#include "ts_game_mode.hpp"

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
    rscManager.loadSprSht("data/tlg_animated.bmp", 12, 11, 131);
	rscManager.loadSprSht("data/objects.bmp", 5, 5, 25);
	rscManager.loadImg("data/room.bmp");
#endif

	// Building scene
	Scene scene;

	// Setting up things
	vect2df_t roomPos;
	roomPos.x = 0.;
	roomPos.y = 0.;

	Sprite room(rscManager.getImgRsc(2), roomPos);
	Font bigFont("data/font-big.bmp", 16, 16, 256);
	Font smallFont("data/font-small.bmp", 16, 16, 256);

	vect2df_t testTextPos;
	testTextPos.x = 10;
	testTextPos.y = 10;

	Text testText("TEST", &bigFont, testTextPos);

	vect2df_t vStorePos;
	vStorePos.x = 290;
	vStorePos.y = 0;

	size2df_t sStoreSize;
	sStoreSize.w = 30;
	sStoreSize.h = 226;

	ThingsStore thingsStore(vStorePos, sStoreSize, 1, 10);
	ThingsManager thingsManager(&thingsStore);

	TSGameMode gameMode(&scene, &thingsManager);

	// Setting up scene
	/*MainCharacter mainChar(rscManager.getImgRsc(0), charPos, &thingsManager);
	mainChar.translate(150, 100, TRANSFORM_ABS);

	MainCharacter mainChar2(rscManager.getImgRsc(0), charPos, &thingsManager);
	mainChar2.translate(100, 200, TRANSFORM_ABS);*/

	scene.addComponent(&room);

	vect2df_t testThingPos;
	testThingPos.x = 0;
	testThingPos.y = 0;

	DraggableThing dt1(rscManager.getSprShtRsc(1), 0, testThingPos, &thingsManager, sys->getInputSys(), 1, 10, 15, 80, true, true, true, true);
	DraggableThing dt2(rscManager.getSprShtRsc(1), 1, testThingPos, &thingsManager, sys->getInputSys(), 1, 8,  20, 80, true, true, true, true);
	DraggableThing dt3(rscManager.getSprShtRsc(1), 2, testThingPos, &thingsManager, sys->getInputSys(), 1, 5,  10, 80, true, true, true, true);

	thingsStore.addWidget(&dt1);
	thingsStore.addWidget(&dt2);
	thingsStore.addWidget(&dt3);

	//
	testThingPos.x = 225;
	testThingPos.y = 10;
	CriticalThing ct1(rscManager.getSprShtRsc(1), 3, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 135;
	CriticalThing ct2(rscManager.getSprShtRsc(1), 3, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 40;
	CriticalThing ct3(rscManager.getSprShtRsc(1), 3, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	//
	testThingPos.x = 225;
	testThingPos.y = 194;
	CriticalThing ct4(rscManager.getSprShtRsc(1), 5, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 135;
	CriticalThing ct5(rscManager.getSprShtRsc(1), 5, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.x = 40;
	CriticalThing ct6(rscManager.getSprShtRsc(1), 5, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	//
	testThingPos.x = 10;
	testThingPos.y = 40;
	CriticalThing ct7(rscManager.getSprShtRsc(1), 6, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 100;
	CriticalThing ct8(rscManager.getSprShtRsc(1), 6, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 160;
	CriticalThing ct9(rscManager.getSprShtRsc(1), 6, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	//
	testThingPos.x = 260;
	testThingPos.y = 40;
	CriticalThing ct10(rscManager.getSprShtRsc(1), 4, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 100;
	CriticalThing ct11(rscManager.getSprShtRsc(1), 4, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	testThingPos.y = 160;
	CriticalThing ct12(rscManager.getSprShtRsc(1), 4, testThingPos, &thingsManager, sys->getInputSys(), 1, -1, false, true);

	thingsManager.addThing(&ct1);
	thingsManager.addThing(&ct2);
	thingsManager.addThing(&ct3);
	thingsManager.addThing(&ct4);
	thingsManager.addThing(&ct5);
	thingsManager.addThing(&ct6);
	thingsManager.addThing(&ct7);
	thingsManager.addThing(&ct8);
	thingsManager.addThing(&ct9);
	thingsManager.addThing(&ct10);
	thingsManager.addThing(&ct11);
	thingsManager.addThing(&ct12);

	
	scene.addComponent(&thingsStore);
	scene.addComponent(&testText);
	scene.addComponent(&ct1);
	scene.addComponent(&ct2);
	scene.addComponent(&ct3);
	scene.addComponent(&ct4);
	scene.addComponent(&ct5);
	scene.addComponent(&ct6);
	scene.addComponent(&ct7);
	scene.addComponent(&ct8);
	scene.addComponent(&ct9);
	scene.addComponent(&ct10);
	scene.addComponent(&ct11);
	scene.addComponent(&ct12);

	// Spawning character(s)
	vect2df_t charPos;
	charPos.x = 150.;
	charPos.y = 100.;

	gameMode.spawnCharacter(charPos);

	charPos.x = 50;
	charPos.y = 150;

	//gameMode.spawnCharacter(charPos);



	// We don't need double buffering in this example.
	// In this way we can draw our image only once on screen.
	gfx->SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx->GetFramebuffer();

	sys->initLoop();

	int i = 0;

	// Main loop
	while (sys->mainLoop())
	{
		double deltaTime = sys->getDeltaTime();

		//printf("FPS: %u ", (uint) (1./deltaTime));

		gfx->FillWithColor(0x00);

		// Scan all the inputs. This should be done once for each frame
		if (sys->getInputSys()->IsJoyBtnPressed(JOY_BTN_START)) break;

		vect2d_t touchPt;
		if (sys->getInputSys()->GetTouch(&touchPt)) {
			scene.receiveTouchInput(touchPt);
		}

		MouseEvent* mouseEvt = sys->getInputSys()->GetButtonPressEvent(MOUSE_BTN_LEFT);

		if (mouseEvt) {
			scene.receiveTouchInput(mouseEvt->position);
		}

		if (sys->getInputSys()->IsKeyPressed(KEYB_Q) || sys->getInputSys()->IsJoyBtnPressed(JOY_LEFT)) {
            //vect2d_t currCamPos = parallaxBG.getCamPos();
			//parallaxBG.setCamPos(currCamPos.x + -1 * deltaTime, 0);
		}
		else if (sys->getInputSys()->IsKeyPressed(KEYB_D) || sys->getInputSys()->IsJoyBtnPressed(JOY_RIGHT)) {
            //vect2d_t currCamPos = parallaxBG.getCamPos();
			//parallaxBG.setCamPos(currCamPos.x + 1 * deltaTime, 0);
		}
        else if (sys->getInputSys()->IsKeyPressed(KEYB_ESCAPE) || sys->getInputSys()->IsJoyBtnPressed(JOY_BTN_START)) {
            break;
        }

		scene.update();
		scene.draw(fb);

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
