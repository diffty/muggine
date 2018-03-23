#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_SDL
#include <SDL2/SDL.h>
#endif

#include "linked_list.hpp"
#include "common_types.hpp"
#include "scene.hpp"
#include "system.hpp"
#include "graphics.hpp"
//#include "sound.hpp"
#include "input.hpp"
#include "rsc_manager.hpp"
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "fsm.hpp"
#include "font.hpp"
#include "text.hpp"
#include "ld_game_manager.hpp"
#include "ld_game_mode.hpp"
#include "ld_main_menu.hpp"

#include <time.h>

#ifdef TARGET_WIN
#include <windows.h>
#endif

#ifdef TARGET_OSX
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#endif

#ifdef __APPLE__
namespace CoreFoundation {
    #include "CoreFoundation/CoreFoundation.h"
}
#endif


void MainApp(System* pSys, Graphics* pGfx) {
#if DEBUG==1
    
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            fprintf(stdout, "Current working dir: %s\n", cwd);
        else
            perror("getcwd() error");
#endif
    
#ifdef __APPLE__
    CoreFoundation::CFBundleRef mainBundle = CoreFoundation::CFBundleGetMainBundle();
    CoreFoundation::CFURLRef resourcesURL = CoreFoundation::CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CoreFoundation::CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (CoreFoundation::UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CoreFoundation::CFRelease(resourcesURL);
    
    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
#endif
    
#ifdef TARGET_3DS
	Result rc = romfsInit();
#endif
    
	RscManager rscManager;

    rscManager.loadFont("data/font-big.bmp", 16, 16, 256, -1);
	rscManager.loadFont("data/font-small.bmp", 16, 16, 256, -1);
	rscManager.loadFont("data/font-small-black.bmp", 16, 16, 256, -1);
	rscManager.loadSprSht("data/main_menu_ui.bmp", 6, 2, 8);
	rscManager.loadImg("data/title.bmp");
	rscManager.loadImg("data/credits.bmp");
	rscManager.loadImg("data/bg.bmp");
	rscManager.loadImg("data/card.bmp");
	rscManager.loadSprSht("data/ui1.bmp", 3, 1, 3);
	rscManager.loadSprSht("data/objects.bmp", 6, 2, 8);
	rscManager.loadImg("data/truck-slot.bmp");
	rscManager.loadSprSht("data/ui2.bmp", 1, 1, 1);
	rscManager.loadSprSht("data/truck.bmp", 3, 1, 3);
	rscManager.loadSprSht("data/truck_wheels.bmp", 1, 1, 1);
	rscManager.loadSprSht("data/transition-stripes.bmp", 2, 1, 2);
	rscManager.loadImg("data/crate.bmp");
	rscManager.loadFont("data/font-pixel.bmp", 16, 16, 256, 1);
	rscManager.loadImg("data/heart.bmp");
	rscManager.loadSprSht("data/particles.bmp", 4, 2, 8);

	// Sound system
	//Sound sound;
	//sound.addSound("data/sound/intro-rix.mp3", true);
	
	// Initing game manager
	LDGameManager gameManager;

	// Building scene
	Scene* pGameScene = gameManager.getGameScene();
	Scene* pMenuScene = gameManager.getMenuScene();

	LDGameMode* pGameMode = NULL;
	
	pGfx->SetDoubleBuffering(false);

	uint8* fb = pGfx->GetFramebuffer();

	pSys->initLoop();

	// Main loop
	while (pSys->mainLoop())
	{
		double deltaTime = pSys->getDeltaTime();

		// printf("FPS: %u\n", (uint) (1./deltaTime));

		pGfx->FillWithColor(0x00);

		gameManager.update();
		gameManager.draw(fb);

		
		//if (sys->getInputSys()->IsKeyPressed(KEYB_A)) { }

		//sound.update();

		// Flush and swap framebuffers
		pGfx->FlushBuffer();
		pGfx->SwapBuffer();

		// Wait for VBlank
		pGfx->WaitForBlank();
	}
	

	rscManager.freeAllRsc();

	// Exit services
	pGfx->Exit();
}

int main(int argc, char **argv)
{
	System* pSys = System::get();
	Graphics gfx(pSys);

#ifdef TARGET_SDL
	pSys->initWindow();
#endif

	gfx.Init();
    pSys->initConsole();  // toujours initialiser la console après l'init de Gfx, surtout pour la 3DS.
    // printf("lol");

	MainApp(pSys, &gfx);

	delete pSys;

	return 0;
}
