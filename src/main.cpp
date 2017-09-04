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
	rscManager.loadRsc("romfs:/data/bg1.bmp");   // Catch le crash quand le path est pas bon stp
	rscManager.loadRsc("romfs:/data/bg2.bmp");
	rscManager.loadRsc("romfs:/data/bg3.bmp");
	rscManager.loadRsc("romfs:/data/bg4.bmp");
#else
	rscManager.loadRsc("data/bg1.bmp");   // Catch le crash quand le path est pas bon stp
	rscManager.loadRsc("data/bg2.bmp");
	rscManager.loadRsc("data/bg3.bmp");
    rscManager.loadRsc("data/bg4.bmp");
    rscManager.loadRsc("data/transptest.bmp");
#endif

	// Building scene
	Scene scene;

	// Creating components
    ParallaxBackground parallaxBG;
    Image* transpImg = rscManager.getImgRsc(4);
    
    // Setting up BG
    parallaxBG.addLayer(rscManager.getImgRsc(3), 4, 0, -20);
    parallaxBG.addLayer(rscManager.getImgRsc(2), 3, 0, -15);
    parallaxBG.addLayer(rscManager.getImgRsc(1), 2, 0, -7);
    parallaxBG.addLayer(rscManager.getImgRsc(0), 0);
    
    parallaxBG.translate(0, 100);
    
	// Setting up scene
	scene.addComponent(&parallaxBG);

	// We don't need double buffering in this example.
	// In this way we can draw our image only once on screen.
	gfx->SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx->GetFramebuffer();

	sys->initLoop();

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

		if (sys->GetInputSys()->IsKeyPressed(KEYB_Q) || sys->GetInputSys()->IsJoyBtnPressed(JOY_LEFT)) {
            vect2d_t currCamPos = parallaxBG.getCamPos();
			parallaxBG.setCamPos(currCamPos.x + -1 * deltaTime, 0);
		}
		else if (sys->GetInputSys()->IsKeyPressed(KEYB_D) || sys->GetInputSys()->IsJoyBtnPressed(JOY_RIGHT)) {
            vect2d_t currCamPos = parallaxBG.getCamPos();
			parallaxBG.setCamPos(currCamPos.x + 1 * deltaTime, 0);
		}
        else if (sys->GetInputSys()->IsKeyPressed(KEYB_ESCAPE) || sys->GetInputSys()->IsJoyBtnPressed(JOY_BTN_START)) {
            break;
        }

		scene.update();
		scene.draw(fb);
        
        transpImg->draw(fb, 50, 50, false, true);
        
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
