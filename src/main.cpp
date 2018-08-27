#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_SDL
#include <SDL/SDL.h>
#elif TARGET_SDL2
#include <SDL2/SDL.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
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
#include "or_game_manager.hpp"
#include "or_game_mode.hpp"
#include "or_main_menu.hpp"

#include <time.h>

#if TARGET_WIN
#include <windows.h>
#endif

#if TARGET_OSX
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

    rscManager.loadFont("data/font-big.bmp", 16, 16, 256, -1);          // 0
	rscManager.loadFont("data/font-small.bmp", 16, 16, 256, -1);        // 1
	rscManager.loadFont("data/font-small-black.bmp", 16, 16, 256, -1);  // 2
    rscManager.loadSprSht("data/main_menu_ui.bmp", 6, 2, 8);            // 3
    rscManager.loadSprSht("data/title.bmp", 4, 2, 7);                   // 4
	rscManager.loadImg("data/credits.bmp");                             // 5
    rscManager.loadImg("data/road.bmp");                                // 6
    rscManager.loadImg("data/car.bmp");                                 // 7
    rscManager.loadImg("data/bg.bmp");                                  // 8
    rscManager.loadSprSht("data/car_smoke.bmp", 3, 1, 3);               // 9
    rscManager.loadImg("data/PickUp_Feuille.bmp");                      // 10
    rscManager.loadImg("data/PickUp_House.bmp");                        // 11
    rscManager.loadImg("data/PickUp_Industrie.bmp");                    // 12
    rscManager.loadImg("data/arbre.bmp");                               // 13
    rscManager.loadSprSht("data/usine.bmp", 1, 1, 1);                   // 14
    rscManager.loadImg("data/immeuble.bmp");                            // 15
    rscManager.loadImg("data/juno_bulle.bmp");                          // 16
    rscManager.loadImg("data/mescouilles.bmp");                         // 17
    rscManager.loadImg("data/tomate.bmp");                              // 18
    rscManager.loadSprSht("data/arbre1SprSht.bmp", 3, 1, 3);            // 19
    rscManager.loadSprSht("data/arbre2SprSht.bmp", 3, 1, 3);            // 20
    rscManager.loadSprSht("data/centrecommercial.bmp", 1, 1, 1);        // 21
    rscManager.loadSprSht("data/immeuble1SprSht.bmp", 3, 1, 3);         // 22
    rscManager.loadSprSht("data/immeuble2SprSht.bmp", 3, 1, 3);         // 23
    rscManager.loadImg("data/montagnes.bmp");                           // 24
    rscManager.loadImg("data/nuages.bmp");                              // 25
    rscManager.loadImg("data/fond.bmp");                                // 26
    rscManager.loadFont("data/font-thin-number.bmp", 16, 16, 256, 2);   // 27
    rscManager.loadImg("data/election_text.bmp");                       // 28
    rscManager.loadImg("data/mayorpopularity.bmp");                     // 29
    rscManager.loadImg("data/pause.bmp");                               // 30
    rscManager.loadImg("data/win.bmp");                                 // 31
    rscManager.loadImg("data/lose.bmp");                                // 32
    rscManager.loadImg("data/population_text.bmp");                     // 33
    
	// Sound system
	//Sound sound;
	//sound.addSound("data/sound/intro-rix.mp3", true);
	
	// Initing game manager
	ORGameManager gameManager;

	// Building scene
	Scene* pGameScene = gameManager.getGameScene();
	Scene* pMenuScene = gameManager.getMenuScene();

	pGfx->SetDoubleBuffering(false);

	uint8* fb = pGfx->GetFramebuffer();

	pSys->initLoop();
    
    // DEBUG 3DS
    //pMenuScene->addComponent(new Sprite(rscManager.getImgRsc(15), { 10, 10 }));
    
	// Main loop
	while (pSys->mainLoop())
	{
		double deltaTime = pSys->getDeltaTime();

		// printf("FPS: %u\n", (uint) (1./deltaTime));

		gameManager.update();
		gameManager.draw(fb);

        // DEBUG 3DS
        if (pSys->getInputSys()->IsJoyBtnPressed(JOY_BTN_1)) {
            gameManager.onNewLevel();
        }
        
		// Flush and swap framebuffers
		pGfx->FlushBuffer();
		pGfx->SwapBuffer();

		// Wait for VBlank
		pGfx->WaitForBlank();
	}
	
	rscManager.freeAllRsc();
}

#ifdef __EMSCRIPTEN__
extern "C" int main(int argc, char** argv) {
#else
int main(int argc, char **argv)
#endif
{
	System* pSys = System::get();
	Graphics gfx(pSys);

#if TARGET_SDL || TARGET_SDL2
	pSys->initWindow();
#endif

	gfx.Init();
    pSys->initConsole();  // toujours initialiser la console après l'init de Gfx, surtout pour la 3DS.
    // printf("lol");

	MainApp(pSys, &gfx);

	// Exit services
	gfx.Exit();
	pSys->exit();

	delete pSys;

	return 0;
}
