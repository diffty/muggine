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
#include "game_manager.hpp"
#include "game_mode.hpp"
#include "xx_main_menu.hpp"
#include "jsonreader.hpp"
#include "scenemanager.hpp"
#include "animation_timeline_widget.hpp"

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
	rscManager.loadFromJSON("data/resources.json");
    
	// Initing game manager
	GameManager gameManager;

	// Building scene
	Scene* pGameScene = gameManager.getGameScene();
	Scene* pMenuScene = gameManager.getMenuScene();

	pGfx->SetDoubleBuffering(false);
	uint8* fb = pGfx->GetFramebuffer();
	pSys->initLoop();
    
    int frameId = 0;
    int x = 0, y = 0;

	SceneManager sceneManager;
	sceneManager.loadFromJSON("data/scene01.json");

	SceneDescription* pCurrSceneDesc = sceneManager.getScene("Scene01");
	Scene* pCurrScene = pCurrSceneDesc->pScene;
	AnimationTimeline* pCurrAnimTimeline = pCurrSceneDesc->pAnimTimeline;

	AnimationTimelineWidget animTimelineWidget(10, 200, 300, 10, pCurrAnimTimeline, 0.0, 10.0);
	pCurrScene->addComponent(&animTimelineWidget);

	Input* pInputSys = pSys->getInputSys();

    // Main loop
	while (pSys->mainLoop())
	{
		double deltaTime = pSys->getDeltaTime();
        
        pGfx->FillWithColor(0x00);

		// printf("FPS: %u\n", (uint) (1./deltaTime));
        
		//gameManager.update();
		//gameManager.draw(fb);

		if (pInputSys->IsKeyPressed(KEYB_Q)) {
			pCurrAnimTimeline->setTime(pCurrAnimTimeline->getTime() - deltaTime);
		}

		if (pInputSys->IsKeyPressed(KEYB_D)) {
			pCurrAnimTimeline->setTime(pCurrAnimTimeline->getTime() + deltaTime);
		}

		if (pInputSys->IsKeyPressed(KEYB_SPACE)) {
			pCurrAnimTimeline->setIsPlaying(!pCurrAnimTimeline->isPlaying());
		}

		MouseEvent* mouseEvt = pInputSys->GetButtonPressEvent(MOUSE_BTN_LEFT);
		if (mouseEvt) {
			animTimelineWidget.receiveTouchInput(pInputSys->getCurrInputPos());
		}

		pCurrScene->update();
		pCurrScene->draw(fb);

		pCurrAnimTimeline->update();

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

	MainApp(pSys, &gfx);

	// Exit services
	gfx.Exit();
	pSys->exit();

	delete pSys;

	return 0;
}
