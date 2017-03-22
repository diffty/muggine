#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_WIN
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



int test() {
	printf("TESTETSTESTSE\n");
	return 0;
}


Uint32 getTime() {
	return SDL_GetTicks();
}

void MainApp(System* sys) {
	Graphics gfx(sys);
	gfx.Init();

	RscManager rscManager;
	rscManager.loadResource("D:/brick.bmp");
	rscManager.loadResource("D:/ball.bmp");
	rscManager.loadResource("D:/paddle.bmp");

	// Building scene
	Scene scene;

	// Creating components
	Grid bkoGrid(0, 0, 10, 7, &rscManager);
	Ball bkoBall(1, &rscManager);
	Paddle bkoPaddle(2, &rscManager);

	// Setting up
	bkoPaddle.translate(180, 200, TRANSFORM_ABS);
	bkoBall.translate(
		bkoPaddle.getRect()->getPos().x + bkoPaddle.getRect()->getSize().w / 2,
		bkoPaddle.getRect()->getPos().y - bkoPaddle.getRect()->getSize().h - 1,
		TRANSFORM_ABS);

	bkoBall.setVelocity(1, 1);

	scene.addComponent(&bkoGrid);
	scene.addComponent(&bkoBall);
	scene.addComponent(&bkoPaddle);

	// We don't need double buffering in this example.
	// In this way we can draw our image only once on screen.
	gfx.SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx.GetFramebuffer();

	printf("Press Start to exit.\n");

	Uint32 deltaTime = 0;
	Uint32 prevTime = getTime();
	Uint32 newTime = prevTime;

	// Main loop
	while (sys->MainLoop())
	{
		
		newTime = getTime();
		deltaTime = newTime - prevTime;
		prevTime = newTime;

		// Scan all the inputs. This should be done once for each frame
		//if (input.IsPressed(KEY_START)) break;

		gfx.FillWithColor(0xFF);

		vect2d_t touchPt;
		if (sys->GetInputSys()->GetTouch(&touchPt)) {
			scene.receiveTouchInput(touchPt);
		}

		MouseEvent* mouseEvt = sys->GetInputSys()->GetButtonPressEvent(MOUSE_BTN_LEFT);

		if (mouseEvt) {
			scene.receiveTouchInput(mouseEvt->position);
		}

		if (sys->GetInputSys()->IsKeyPressed(KEY_Z)) {
			bkoPaddle.translate(0, -1 * deltaTime);
		}
		if (sys->GetInputSys()->IsKeyPressed(KEY_Q)) {
			bkoPaddle.translate(-1 * deltaTime, 0);
		}
		if (sys->GetInputSys()->IsKeyPressed(KEY_S)) {
			bkoPaddle.translate(0, 1 * deltaTime);
		}
		if (sys->GetInputSys()->IsKeyPressed(KEY_D)) {
			bkoPaddle.translate(1 * deltaTime, 0);
		}

		scene.update();
		scene.draw(fb);

		// Flush and swap framebuffers
		gfx.FlushBuffer();
		gfx.SwapBuffer();

		// Wait for VBlank
		gfx.WaitForBlank();
	}

	// RtpMidi::shutdownService();

	// Exit services
	gfx.Exit();
}

int main(int argc, char **argv)
{
	System sys;

	// Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	sys.ConsoleInit();

#ifdef TARGET_WIN
    sys.InitWindow();
#endif

	MainApp(&sys);

	return 0;
}
