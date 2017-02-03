#include <3ds.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "samplergridwidget.hpp"
#include "linked_list.hpp"
#include "common_types.hpp"
#include "scene.hpp"
#include "buttonwidget.hpp"



int main(int argc, char **argv)
{
	gfxInitDefault();

	// Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, NULL);

	// Building UI
	Scene scene;

	ButtonWidget btn(5, 211, 50, 25, 0);
	ButtonWidget btn2(70, 211, 50, 25, 0);

	SamplerGridWidget sg(10, 10, 300, 200, 10, 8);

	scene.addComponent((IWidget*) &sg);
	scene.addComponent((IWidget*) &btn);
	scene.addComponent((IWidget*) &btn2);

	// We don't need double buffering in this example. In this way we can draw our image only once on screen.
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	// Get the bottom screen's frame buffer
	u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

	printf("\x1b[20;15HPress Start to exit.");

	// Main loop
	while (aptMainLoop())
	{
		// Scan all the inputs. This should be done once for each frame
		hidScanInput();

		// hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		// Reading touch input if any
		touchPosition touch;
		hidTouchRead(&touch);

		if (touch.px + touch.py > 0) {
			vect2d_t touchPt;

			touchPt.x = touch.px;
			touchPt.y = touch.py;

			scene.receiveTouchInput(touchPt);
		}

		scene.update();
		scene.draw(fb);

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		// Wait for VBlank
		gspWaitForVBlank();

	}

	// Exit services
	gfxExit();
	return 0;
}
