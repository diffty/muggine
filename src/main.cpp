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

#include "utils/linked_list.hpp"
#include "core/common_types.hpp"
#include "graphics/scene.hpp"
#include "core/system.hpp"
#include "core/graphics.hpp"
//#include "core/sound.hpp"
#include "core/input.hpp"
#include "manager/rsc_manager.hpp"
#include "graphics/sprite.hpp"
#include "graphics/spritesheet.hpp"
#include "ia/fsm.hpp"
#include "ui/widget/text.hpp"
#include "manager/game_manager.hpp"
#include "game/game_mode.hpp"
#include "game/xx_main_menu.hpp"
#include "utils/jsonreader.hpp"
#include "manager/scenemanager.hpp"
#include "ui/widget/animation_timeline_widget.hpp"
#include "ui/widget/outliner_widget.hpp"
#include "ui/floating_window.hpp"
#include "graphics/font_bmp.hpp"
#include "graphics/font_ttf.hpp"


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
	Scene uiScene;

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

	AnimationTimelineWidget* animTimelineWidget = new AnimationTimelineWidget(0, 0, 300, 10, pCurrAnimTimeline, 0.0, 10.0);

	Input* pInputSys = pSys->getInputSys();

	OutlinerWidget* outlinerWidget = new OutlinerWidget(0, 0, 70, 100, pCurrScene);

	FloatingWindow outlinerWindow(30, 100, 70, 100, outlinerWidget);
	outlinerWindow.setParentWidget(&uiScene);

	FloatingWindow timelineWindow(20, 200, 290, 20, animTimelineWidget);
	timelineWindow.setParentWidget(&uiScene);

	FontTTF font("c:/windows/fonts/arialbd.ttf");

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
			vect2d_t vCurrMousePos = pInputSys->getCurrInputPos();
			uiScene.receiveTouchInput(vCurrMousePos);
		}

		pCurrScene->update();
		pCurrScene->draw(fb);

		uiScene.update();
		uiScene.draw(fb);

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
