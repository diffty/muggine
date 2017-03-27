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


void MainApp(System* sys) {
	Graphics gfx(sys);
	gfx.Init();

	RscManager rscManager;
	rscManager.loadResource("data/brick.bmp");
	rscManager.loadResource("data/ball.bmp");
	rscManager.loadResource("data/paddle.bmp");

	// Building scene
	Scene scene;

	// Creating components
	Grid bkoGrid(0, 0, 12, 7, &rscManager);
	Ball bkoBall(1, sys, &rscManager);
	Paddle bkoPaddle(2, &rscManager);

	// Setting up
	bkoPaddle.translate(180, 200, TRANSFORM_ABS);
	bkoBall.translate(
		bkoPaddle.getRect()->getPos().x + bkoPaddle.getRect()->getSize().w / 2,
		bkoPaddle.getRect()->getPos().y - bkoPaddle.getRect()->getSize().h - 1,
		TRANSFORM_ABS);

	bkoBall.setVelocity(1, -1);

	scene.addComponent(&bkoGrid);
	scene.addComponent(&bkoBall);
	scene.addComponent(&bkoPaddle);

	// We don't need double buffering in this example.
	// In this way we can draw our image only once on screen.
	gfx.SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx.GetFramebuffer();

	printf("Press Start to exit.\n");

	sys->initLoop();

	// Main loop
	while (sys->MainLoop())
	{
		Uint32 deltaTime = sys->getDeltaTime();

		if (deltaTime > 1) printf("%d\n", deltaTime);

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

		if (sys->GetInputSys()->IsKeyPressed(KEY_Q)) {
			bkoPaddle.translate(-1 * deltaTime, 0);
		}
		if (sys->GetInputSys()->IsKeyPressed(KEY_D)) {
			bkoPaddle.translate(1 * deltaTime, 0);
		}

		vect2d_t ballPos = bkoBall.getRect()->getPos();
		size2d_t ballSize = bkoBall.getRect()->getSize();
		vect2d_t ballVel = bkoBall.getVelocity();
		vect2d_t ballCenter = bkoBall.getCenterPos();

		uint collidingBrickId;
		uint collisionBrickSide;

		vect2d_t nextBallPos;
		nextBallPos.x = ballPos.x + ballVel.x;
		nextBallPos.y = ballPos.y + ballVel.y;

		vect2d_t nextBallCenterPos;
		nextBallCenterPos.x = ballCenter.x + ballVel.x;
		nextBallCenterPos.y = ballCenter.y + ballVel.y;

		if (nextBallPos.x <= 0 || nextBallPos.x + ballSize.w >= SCREEN_WIDTH) {
			ballVel.x = -ballVel.x;
			printf("cacaX");
		}

		if (nextBallPos.y <= 0 || nextBallPos.y + ballSize.h >= SCREEN_HEIGHT) {
			ballVel.y = -ballVel.y;
			printf("cacaY");
		}

		//printf("%d, %d\n", ballPos.x, ballPos.y);

		if (bkoGrid.checkBrickAtPos(nextBallCenterPos, &collidingBrickId)) {
            //printf("Colliding with %d\n", collidingBrickId);

			Brick* collidingBrick = bkoGrid.getBrickFromId(collidingBrickId);
			collisionBrickSide = collidingBrick->getBallSideFromBrick(&bkoBall);

            switch (collisionBrickSide) {
                case 0:		// LEFT
                    ballVel.x = -ballVel.x;
                    break;
                    
                case 1:		// RIGHT
                    ballVel.x = -ballVel.x;
                    break;

                case 2:		// TOP
                    ballVel.y = -ballVel.y;
                    break;
                    
                case 3:		// BOTTOM
                    ballVel.y = -ballVel.y;
                    break;
            }

			bkoBall.setVelocity(ballVel);
            bkoGrid.getBrickFromId(collidingBrickId)->setActive(false);

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
