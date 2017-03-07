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
//#include "rtpmidi.hpp"


#ifndef TARGET_3DS
#define KEY_START 0x21212121
#endif



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

	scene.addComponent((IWidget*) &sg);
	scene.addComponent((IWidget*) &btn);
	scene.addComponent((IWidget*) &btn2);

	// We don't need double buffering in this example. In this way we can draw our image only once on screen.
	gfx.SetDoubleBuffering(false);

	// Get the bottom screen's frame buffer
	uint8* fb = gfx.GetFramebuffer();

	printf("Press Start to exit.\n");

	// Main loop
	while (sys->MainLoop())
	{
		// Scan all the inputs. This should be done once for each frame
		//if (input.IsPressed(KEY_START)) break;

		vect2d_t touchPt;
		if (sys->GetInputSys()->GetTouch(&touchPt)) {
			scene.receiveTouchInput(touchPt);
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
