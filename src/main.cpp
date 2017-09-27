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
#include "ts_main_character.hpp"
#include "ts_interactable_thing.hpp"
#include "ts_things_manager.hpp"
#include "ts_draggable_thing.hpp"
#include "ts_things_store.hpp"
#include "font.hpp"
#include "text.hpp"

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
    rscManager.loadImg("data/thomas.bmp");
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

	// Setting up scene
	vect2df_t charPos;
	charPos.x = 150.;
	charPos.y = 100.;

	MainCharacter mainChar(rscManager.getImgRsc(0), charPos, &thingsManager);
	mainChar.translate(150, 100, TRANSFORM_ABS);

	vect2df_t testThingPos;
	testThingPos.x = 200;
	testThingPos.y = 10;

	DraggableThing dt1(rscManager.getSprShtRsc(1), 0, testThingPos, &thingsManager, sys->getInputSys(), 1, 5, true, true);
	DraggableThing dt2(rscManager.getSprShtRsc(1), 1, testThingPos, &thingsManager, sys->getInputSys(), 1, 5, true, true);
	DraggableThing dt3(rscManager.getSprShtRsc(1), 2, testThingPos, &thingsManager, sys->getInputSys(), 1, 5, true, true);

	thingsStore.addWidget(&dt1);
	thingsStore.addWidget(&dt2);
	thingsStore.addWidget(&dt3);

	scene.addComponent(&room);
	scene.addComponent(&mainChar);
	scene.addComponent(&thingsStore);
	scene.addComponent(&testText);

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
