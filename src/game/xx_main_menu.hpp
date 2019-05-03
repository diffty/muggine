#ifndef LD_MAIN_MENU_HPP
#define LD_MAIN_MENU_HPP

#include "../graphics/scene.hpp"
#include "../graphics/animatedsprite.hpp"
#include "../ui/widget/image_button_widget.hpp"


class XXMainMenu
{
private:
	Scene* m_pScene;

	ImageButtonWidget* m_pContinueBtn;
	ImageButtonWidget* m_pNewBtn;
	ImageButtonWidget* m_pQuitBtn;

public:
	XXMainMenu(Scene* pScene);
	~XXMainMenu();

	void initScene();

	void setContinueBtnCallback(void(*pCallback)(void*), void* pCallbackArg = NULL);
	void setNewBtnCallback(void(*pCallback)(void*), void* pCallbackArg = NULL);
	void setQuitBtnCallback(void(*pCallback)(void*), void* pCallbackArg = NULL);

	void showContinueBtn(bool bIsBtnVisible);
};


#endif