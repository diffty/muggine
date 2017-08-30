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
//#include "rtpmidi.hpp"

#include "bko_grid.hpp"
#include "bko_ball.hpp"
#include "bko_paddle.hpp"

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
	rscManager.loadRsc("romfs:/data/brick.bmp");   // Catch le crash quand le path est pas bon stp
	rscManager.loadRsc("romfs:/data/ball.bmp");
    rscManager.loadRsc("romfs:/data/paddle.bmp");
	rscManager.loadRsc("romfs:/data/frame.bmp");
#else
    rscManager.loadRsc("data/brick.bmp");   // Catch le crash quand le path est pas bon stp
    rscManager.loadRsc("data/ball.bmp");
    rscManager.loadRsc("data/paddle.bmp");
	rscManager.loadRsc("data/frame.bmp");
#endif
    
	// Building scene
	Scene scene;
    
	// Creating components
	Grid bkoGrid(12, 12, 8, 7, &rscManager);
	Ball bkoBall(1, sys, &rscManager);
	Paddle bkoPaddle(2, &rscManager);
	Sprite bkoFrame(3, &rscManager);

	// Setting up
	bkoFrame.translate(0, 0, TRANSFORM_ABS);
	bkoPaddle.translate(180, 200, TRANSFORM_ABS);
	bkoBall.reinit(&bkoPaddle);

	bkoBall.setVelocity(1, -1);

    //scene.addComponent(&bkoFrame);
	//scene.addComponent(&bkoGrid);
	//scene.addComponent(&bkoBall);
	//scene.addComponent(&bkoPaddle);

	// We don't need double buffering in this example.
	// In this way we can draw our image only once on screen.
	gfx->SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx->GetFramebuffer();

	sys->initLoop();

	int timeBeforeBallMove = 0;
	int moveBallEveryTime = 8;

	int testN = 0;

	// Main loop
	while (sys->MainLoop())
	{
		uint32 deltaTime = sys->getDeltaTime();

		gfx->FillWithColor(0x00);

		// Scan all the inputs. This should be done once for each frame
		if (sys->GetInputSys()->IsJoyBtnPressed(JOY_BTN_START)) break;

		vect2d_t touchPt;
		if (sys->GetInputSys()->GetTouch(&touchPt)) {
			scene.receiveTouchInput(touchPt);
		}

		MouseEvent* mouseEvt = sys->GetInputSys()->GetButtonPressEvent(MOUSE_BTN_LEFT);

		if (mouseEvt) {
			scene.receiveTouchInput(mouseEvt->position);
		}

		if (sys->GetInputSys()->IsKeyPressed(KEYB_A) || testN < 1000) {
			Ball* bkoBall = new Ball(1, sys, &rscManager);
			bkoBall->translate((float) rand() / RAND_MAX * 300, (float)rand() / RAND_MAX * 200, TRANSFORM_ABS);
			scene.addComponent(bkoBall);
			printf("add: %i\n", scene.getComponentCount());
			rscManager.loadRsc("data/brick.bmp");
		}
		else if (scene.getComponentCount() == 0 && testN > 0) {
			testN = 0;
		}
		else if (sys->GetInputSys()->IsKeyPressed(KEYB_Z) || testN >= 1000) {
			Ball* ballObj = (Ball*) scene.getFirstComponent();
			scene.removeComponent(scene.getFirstComponent());
			delete ballObj;
			printf("remove: %i\n", scene.getComponentCount());
			rscManager.unloadRsc(rscManager.getRscCount()-1);
		}

		scene.update();
		scene.draw(fb);

		// Flush and swap framebuffers
		gfx->FlushBuffer();
		gfx->SwapBuffer();

		// Wait for VBlank
		gfx->WaitForBlank();

		testN++;
	}

	// RtpMidi::shutdownService();

    rscManager.freeAllRsc();

	// Exit services
	gfx->Exit();
}

int main(int argc, char **argv)
{
	System sys;
    Graphics gfx(&sys);
    
#ifdef TARGET_SDL
    sys.InitWindow();
#endif

    gfx.Init();
    sys.ConsoleInit();  // toujours initialiser la console après l'init de Gfx, surtout pour la 3DS.
    
	MainApp(&sys, &gfx);
    
    return 0;
}

