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

#define STB_TRUETYPE_IMPLEMENTATION

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
#include "graphics/drawbuffer.hpp"
#include "graphics/drawing.hpp"
#include "game/ct_character.hpp"
#include "game/ct_wagon.hpp"
#include "game/ct_train.hpp"

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
	drawbuffer fb = pGfx->GetFramebuffer();
	pSys->initLoop();
    
    int frameId = 0;
    int x = 0, y = 0;

	//SceneManager sceneManager;
	//sceneManager.loadFromJSON("data/scene01.json");

	Input* pInputSys = pSys->getInputSys();

    #ifdef __APPLE__
        FontTTF font("/System/Library/Fonts/SFCompactDisplay.ttf");
    #else
        FontTTF font("c:/windows/fonts/arialbd.ttf");
    #endif
    
    Color bgClr(200, 100, 0);

    SpriteSheet* chaSprSht = rscManager.getSprShtRsc("cha");
    SpriteSheet* fredSprSht = rscManager.getSprShtRsc("fred");
    
    AnimatedSprite cha(chaSprSht, 0, 0);
    AnimatedSprite fred(fredSprSht, 0, 0);
    
    CTTrain train;
    pGameScene->addComponent(&train);
    //pGameScene->addComponent(&cha);
    //pGameScene->addComponent(&fred);

    train.addWagon();
    train.addTransition();
    train.addWagon();
    train.addTransition();
    train.addWagon();
    train.addTransition();

    train.translate(fb.width / 2 - train.getRect()->getSize().w / 2, 0, TRANSFORM_ABS);

    //CTWagon* wagon = new CTWagon();
    //pGameScene->addChildWidget(wagon);
    
    Image* m_pTransitionWagonImg = rscManager.getImgRsc("transition-wagon");
    
    // Main loop
	while (pSys->mainLoop())
	{
		double deltaTime = pSys->getDeltaTime();
        
        pGfx->FillWithColor(0x00);

        float speed = 200.;
        
        train.translate(0, -speed * deltaTime, TRANSFORM_REL);
        
        // printf("FPS: %u\n", (uint) (1./deltaTime));
        if (pInputSys->IsKeyPressed(KEYB_Z)) {
            train.translate(0, -speed * deltaTime, TRANSFORM_REL);
        }
        else if (pInputSys->IsKeyPressed(KEYB_S)) {
            train.translate(0, speed * deltaTime, TRANSFORM_REL);
        }

		pGameScene->update();
		pGameScene->draw(&fb);

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
