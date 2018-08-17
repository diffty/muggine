#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef TARGET_3DS
#include <3ds.h>
#elif TARGET_SDL
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

#include "jnl_app_manager.hpp"

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

    rscManager.loadFont("data/font-big.bmp", 16, 16, 256, -1);          // 0
	rscManager.loadFont("data/font-small.bmp", 16, 16, 256, -1);        // 1
	rscManager.loadFont("data/font-small-black.bmp", 16, 16, 256, -1);  // 2
    
	// Initing game manager
	JournalAppManager appManager;

	// Building scene
	Scene* pGameScene = appManager.getScene();

	pGfx->SetDoubleBuffering(false);

	uint8* fb = pGfx->GetFramebuffer();

	pSys->initLoop();
    
    // DEBUG 3DS
    //pMenuScene->addComponent(new Sprite(rscManager.getImgRsc(15), { 10, 10 }));
    
	// Main loop
	while (pSys->mainLoop())
	{
		double deltaTime = pSys->getDeltaTime();

		appManager.update();
		appManager.draw(fb);
        
        MouseEvent* mouseEvt = pSys->getInputSys()->GetButtonPressEvent(MOUSE_BTN_LEFT);
        
        if (mouseEvt) {
            appManager.receiveTouchInput(mouseEvt->position);
        }

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

#ifdef __EMSCRIPTEN__
extern "C" int main(int argc, char** argv) {
#else
int main(int argc, char **argv)
#endif
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
