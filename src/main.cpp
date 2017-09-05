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
    rscManager.loadRsc("data/simon2.bmp");
	rscManager.loadRsc("data/animtest3.bmp");
#endif

	// Building scene
	Scene scene;

	// Creating components
    ParallaxBackground parallaxBG;
    Image* transpImg = rscManager.getImgRsc(4);
	SpriteSheet sprSht("data/animtest3.bmp", 2, 2, 4);
    
    // Setting up BG
	parallaxBG.addLayer(rscManager.getImgRsc(4), 7, 0, -48);
	parallaxBG.addLayer(rscManager.getImgRsc(4), 6, 0, -47);
	parallaxBG.addLayer(rscManager.getImgRsc(4), 5, 0, -45);
	parallaxBG.addLayer(rscManager.getImgRsc(4), 4, 0, -40);
    parallaxBG.addLayer(rscManager.getImgRsc(4), 3, 0, -30);
    parallaxBG.addLayer(rscManager.getImgRsc(4), 2, 0, -15);
    parallaxBG.addLayer(rscManager.getImgRsc(4), 1.5);
    
    parallaxBG.translate(0, 170);
    
	// Setting up scene
	scene.addComponent(&parallaxBG);

	// We don't need double buffering in this example.
	// In this way we can draw our image only once on screen.
	gfx->SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx->GetFramebuffer();

	sys->initLoop();

	int i = 0;

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

		sprSht.draw(fb, -10, -10, false, true);
		sprSht.draw(fb, -10, 230, false, true);
		sprSht.draw(fb, 300, 220, false, true);
		sprSht.draw(fb, 300, -10, false, true);
		sprSht.draw(fb, 120, 120, false, true);

		//transpImg->draw(fb, -20, -20, 0, 0, 68, 100, false, true);
		
		if (i % 1500 == 0) sprSht.nextFrame();
        
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
