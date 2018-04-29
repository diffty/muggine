#ifndef LD_MAIN_MENU_HPP
#define LD_MAIN_MENU_HPP

#include "scene.hpp"
#include "animatedsprite.hpp"
#include "image_button_widget.hpp"


class ORMainMenu
{
private:
	Scene* m_pScene;

	ImageButtonWidget* m_pContinueBtn;
	ImageButtonWidget* m_pNewBtn;
	ImageButtonWidget* m_pQuitBtn;

public:
	ORMainMenu(Scene* pScene);
	~ORMainMenu();

	void initScene();

	void setContinueBtnCallback(void(*pCallback)(void*), void* pCallbackArg = NULL);
	void setNewBtnCallback(void(*pCallback)(void*), void* pCallbackArg = NULL);
	void setQuitBtnCallback(void(*pCallback)(void*), void* pCallbackArg = NULL);

	void showContinueBtn(bool bIsBtnVisible);
};


#endif
