#ifndef LD_GAME_MODE_HPP
#define LD_GAME_MODE_HPP

#include <stdio.h>

#include "scene.hpp"
#include "linked_list.hpp"
#include "text.hpp"
#include "csv_reader.hpp"
#include "common_tools.hpp"
#include "platform_tools.hpp"
#include "sprite.hpp"
#include "input.hpp"
#include "text.hpp"

#include "particle_system.hpp"
#include "progress_bar.hpp"

#include "drawing.hpp"


class GameMode
{
private:
	static GameMode* s_pInstance;

	Scene* m_pMainScene;

	float m_fTimer;


	LinkedList m_llWidgetTrash;


public:
	GameMode(Scene* pMainScene);
	~GameMode();

	void initScene();

    void update();
    void debugDraw(uint8* buffer);
    
	Scene* getScene();
    
    void destroyWidget(IWidget* pWidget);
	void destroyWidgetsInTrash();

	static GameMode* get();

	static void initGameMode(GameMode** pGameMode, Scene* pScene);
};


#endif
