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
    rscManager.loadRsc("romfs:/data/animtest2.bmp");
#else
	rscManager.loadRsc("data/brick.bmp");   // Catch le crash quand le path est pas bon stp
	rscManager.loadRsc("data/ball.bmp");
	rscManager.loadRsc("data/paddle.bmp");
    rscManager.loadRsc("data/frame.bmp");
    rscManager.loadRsc("data/animtest2.bmp");
#endif


	// Building scene
	Scene scene;

	// Creating components
	Grid bkoGrid(12, 12, 8, 7, &rscManager);
	Ball bkoBall(1, sys, &rscManager);
	Paddle bkoPaddle(2, &rscManager);
	Sprite bkoFrame(3, &rscManager);
    
    #ifdef TARGET_3DS
    SpriteSheet sprSht("romfs:/data/animtest2.bmp", 2, 2, 4);
    #else
    SpriteSheet sprSht("data/animtest2.bmp", 2, 2, 4);
    #endif

	// Setting up
	bkoFrame.translate(0, 0, TRANSFORM_ABS);
	bkoPaddle.translate(180, 200, TRANSFORM_ABS);
	bkoBall.reinit(&bkoPaddle);

	bkoBall.setVelocity(1, -1);

	scene.addComponent(&bkoFrame);
	scene.addComponent(&bkoGrid);
	scene.addComponent(&bkoBall);
	scene.addComponent(&bkoPaddle);

	// We don't need double buffering in this example.
	// In this way we can draw our image only once on screen.
	gfx->SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx->GetFramebuffer();

	sys->initLoop();

	int timeBeforeBallMove = 0;
	int moveBallEveryTime = 8;

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
			bkoPaddle.translate(-1 * deltaTime, 0);
		}
		else if (sys->GetInputSys()->IsKeyPressed(KEYB_D) || sys->GetInputSys()->IsJoyBtnPressed(JOY_RIGHT)) {
			bkoPaddle.translate(1 * deltaTime, 0);
		}
        else if (sys->GetInputSys()->IsKeyPressed(KEYB_ESCAPE) || sys->GetInputSys()->IsJoyBtnPressed(JOY_BTN_START)) {
            break;
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
			ballVelVect.x = ballVel.x; // * (long)deltaTime;
			ballVelVect.y = ballVel.y; // * (long)deltaTime;

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

				if (collisionPoint.y >= GAMEZONE_BOTTOM - 1) {
					bkoBall.setIsMoving(false);
					bkoBall.setIsDead(true);
				}

				ballVelVect.x = ballVel.x; // * (long)deltaTime;
				ballVelVect.y = ballVel.y; // * (long)deltaTime;
			}

			nextBallPos.x = nextBallCenterPos.x - ballSize.w / 2;
			nextBallPos.y = nextBallCenterPos.y - ballSize.h / 2;

			bkoBall.getRect()->setPos(nextBallPos.x, nextBallPos.y);
			bkoBall.setVelocity(ballVel);
		}

		if (bkoBall.isMoving())
			timeBeforeBallMove -= deltaTime;

		if (bkoBall.isDead() && (sys->GetInputSys()->IsKeyPressed(KEYB_Q) || sys->GetInputSys()->IsKeyPressed(KEYB_D) || sys->GetInputSys()->IsJoyBtnPressed(JOY_LEFT) || sys->GetInputSys()->IsJoyBtnPressed(JOY_RIGHT))) {
			bkoBall.reinit(&bkoPaddle);
		}

		if (bkoBall.isStickToPaddle()) {
			if (sys->GetInputSys()->IsKeyPressed(KEYB_Z) || sys->GetInputSys()->IsJoyBtnPressed(JOY_UP)) {
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
        
        sprSht.draw(fb, 100, 100, false, false);
        sprSht.nextFrame();

		// Flush and swap framebuffers
		gfx->FlushBuffer();
		gfx->SwapBuffer();

		// Wait for VBlank
		gfx->WaitForBlank();
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
