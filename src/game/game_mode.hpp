#ifndef LD_GAME_MODE_HPP
#define LD_GAME_MODE_HPP

#include <stdio.h>

#include "../graphics/scene.hpp"
#include "../utils/linked_list.hpp"
#include "../ui/widget/text.hpp"
#include "../utils/csv_reader.hpp"
#include "../core/common_tools.hpp"
#include "../core/platform_tools.hpp"
#include "../graphics/sprite.hpp"
#include "../core/input.hpp"
#include "../ui/widget/text.hpp"

#include "../graphics/particle_system.hpp"
#include "../ui/widget/progress_bar.hpp"

#include "../graphics/drawing.hpp"


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
    void debugDraw(drawbuffer* pBuffer);
    
	Scene* getScene();
    
    void destroyWidget(IWidget* pWidget);
	void destroyWidgetsInTrash();

	static GameMode* get();

	static void initGameMode(GameMode** pGameMode, Scene* pScene);
};


#endif
