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
	bkoBall.reinit(&bkoPaddle);

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

	int timeBeforeBallMove = 0;
	int moveBallEveryTime = 20;

	// Main loop
	while (sys->MainLoop())
	{
		Uint32 deltaTime = sys->getDeltaTime();

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

		if (bkoBall.isMoving() && timeBeforeBallMove <= 0) {
			timeBeforeBallMove = moveBallEveryTime;

			uint collidingBrickId;
			int collisionBrickSide = -1;

			vect2d_t ballVelVect;
			ballVelVect.x = ballVel.x/* * (long)deltaTime*/;
			ballVelVect.y = ballVel.y/* * (long)deltaTime*/;

			vect2d_t nextBallPos;
			nextBallPos.x = ballPos.x + ballVelVect.x;
			nextBallPos.y = ballPos.y + ballVelVect.y;

			vect2d_t nextBallCenterPos;
			nextBallCenterPos.x = ballCenter.x + ballVelVect.x;
			nextBallCenterPos.y = ballCenter.y + ballVelVect.y;

			vect2d_t collisionPoint;
			int collisionType = -1;

			if (bkoBall.checkCollisionBetweenPos(&bkoGrid, &bkoPaddle, ballCenter, nextBallCenterPos, &collidingBrickId, &collisionPoint, &collisionType))
			{
				nextBallCenterPos.x = collisionPoint.x - ballVel.x;
				nextBallCenterPos.y = collisionPoint.y - ballVel.y;

				if (collisionType == 1) {
					ballVel.x = -ballVel.x;
				}
				else if (collisionType == 2) {
					ballVel.y = -ballVel.y;
				}
				else if (collisionType == 3) {
					ballVel.x = -ballVel.x;
					ballVel.y = -ballVel.y;
				}
				else if (collisionType == 4) {
					ballVel.y = -ballVel.y;
				}
				else if (collisionType == 0) {
					Brick* collidingBrick = bkoGrid.getBrickFromId(collidingBrickId);

					vect2d_t brickPos = collidingBrick->getRect()->getPos();
					size2d_t brickSize = collidingBrick->getRect()->getSize();

					if (collisionPoint.y == brickPos.y || collisionPoint.y == brickPos.y + brickSize.h) {
						collisionBrickSide = 1;
					}
					if (collisionPoint.x == brickPos.x || collisionPoint.x == brickPos.x + brickSize.w) {
						if (collisionBrickSide == 1) collisionBrickSide = 2;
						else collisionBrickSide = 0;
					}

					switch (collisionBrickSide) {
					case 0:		// X
						ballVel.x = -ballVel.x;
						break;

					case 1:		// Y
						ballVel.y = -ballVel.y;
						break;

					case 2:		// BOTH
						ballVel.x = -ballVel.x;
						ballVel.y = -ballVel.y;
						break;
					}

					bkoGrid.getBrickFromId(collidingBrickId)->setActive(false);
				}

				if (collisionPoint.y >= SCREEN_HEIGHT-1) {
					bkoBall.setIsMoving(false);
					bkoBall.setIsDead(true);
				}

				ballVelVect.x = ballVel.x/* * (long)deltaTime*/;
				ballVelVect.y = ballVel.y/* * (long)deltaTime*/;
			}

			nextBallPos.x = nextBallCenterPos.x - ballSize.w / 2;
			nextBallPos.y = nextBallCenterPos.y - ballSize.h / 2;

			bkoBall.getRect()->setPos(nextBallPos.x, nextBallPos.y);
			bkoBall.setVelocity(ballVel);
		}

		if (bkoBall.isMoving())
			timeBeforeBallMove -= deltaTime;

		if (bkoBall.isDead() && (sys->GetInputSys()->IsKeyPressed(KEY_Q) || sys->GetInputSys()->IsKeyPressed(KEY_D))) {
			bkoBall.reinit(&bkoPaddle);
		}

		if (bkoBall.isStickToPaddle()) {
			if (sys->GetInputSys()->IsKeyPressed(KEY_Z)) {
				bkoBall.setIsStickToPaddle(false);
				bkoBall.setIsMoving(true);
			}
			else {
				bkoBall.translate(
					bkoPaddle.getRect()->getPos().x + bkoPaddle.getRect()->getSize().w / 2,
					bkoPaddle.getRect()->getPos().y - bkoPaddle.getRect()->getSize().h - 1,
					TRANSFORM_ABS);
			}
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

#ifdef TARGET_SDL
    sys.InitWindow();
#endif

	MainApp(&sys);

	return 0;
}
