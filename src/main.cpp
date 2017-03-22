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



int test() {
	printf("TESTETSTESTSE\n");
	return 0;
}


void MainApp(System* sys) {
	Graphics gfx(sys);

	//RtpMidi::initService();	
	//RtpMidi rtpObj;

	gfx.Init();

	// Building UI
	Scene scene;

	ButtonWidget btn(5, 211, 50, 25, 0, test);
	ButtonWidget btn2(70, 211, 50, 25, 0, test);

	SamplerGridWidget sg(10, 10, 300, 200, 10, 8);

	RscManager rscManager;
	rscManager.loadResource("D:/test.bmp");

	scene.addComponent((IWidget*) &sg);
	scene.addComponent((IWidget*) &btn);
	scene.addComponent((IWidget*) &btn2);

	// We don't need double buffering in this example. In this way we can draw our image only once on screen.
	gfx.SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx.GetFramebuffer();

	printf("Press Start to exit.\n");

	Image* img = rscManager.getImgResource(0);

	Sprite spr(0, &rscManager); 

	scene.addComponent((IWidget*) &spr);

	// Main loop
	while (sys->MainLoop())
	{
		// Scan all the inputs. This should be done once for each frame
		//if (input.IsPressed(KEY_START)) break;

		vect2d_t touchPt;
		if (sys->GetInputSys()->GetTouch(&touchPt)) {
			scene.receiveTouchInput(touchPt);
		}

		MouseEvent* mouseEvt = sys->GetInputSys()->GetButtonPressEvent(MOUSE_BTN_LEFT);

		if (mouseEvt) {
			scene.receiveTouchInput(mouseEvt->position);
		}

		if (sys->GetInputSys()->IsKeyPressed(KEY_Z)) {
			spr.translate(0, -1);
		}
		if (sys->GetInputSys()->IsKeyPressed(KEY_Q)) {
			spr.translate(-1, 0);
		}
		if (sys->GetInputSys()->IsKeyPressed(KEY_S)) {
			spr.translate(0, 1);
		}
		if (sys->GetInputSys()->IsKeyPressed(KEY_D)) {
			spr.translate(1, 0);
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
