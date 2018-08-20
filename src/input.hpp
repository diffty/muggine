#ifndef INPUT_HPP
#define INPUT_HPP


#ifdef TARGET_3DS
#include <3ds.h>

#elif TARGET_SDL
#include <SDL/SDL.h>
#define SDL_Keysym SDL_keysym

#elif TARGET_SDL2
#include <SDL2/SDL.h>

#else
#define _CRT_SECURE_NO_WARNINGS

#endif


#include <stdio.h>
#include <ctime>
#include "common_types.hpp"
#include "keys.hpp"
#include "linked_list.hpp"
#include "constants.hpp"


enum EInputType {
    IN_KEYB,
    IN_MOUSE,
    IN_TOUCH
};

enum EMouseButton {
    MOUSE_BTN_LEFT,
	MOUSE_BTN_MIDDLE,
	MOUSE_BTN_RIGHT,
	MOUSE_BTN_UNKNOWN
};

enum EMouseEventType {
	MOUSE_EVT_MOTION,
	MOUSE_EVT_BTN_UP,
	MOUSE_EVT_BTN_DOWN
};

struct KeyEvent {
    EKey key;
};

struct MouseEvent {
	EMouseEventType type;
    EMouseButton btn;
    vect2d_t position;
};

struct InputEvent {
    int time;
    void* event;
	EInputType type;
};


class Input {
private:
    LinkedList currEventsList;
	LinkedList currInputList;
	LinkedList currMouseList;

	vect2d_t vCurrMousePos;

public:
	Input();
	void ScanInput();
	//uint32 GetInput();
	bool IsPressed(uint32 key);
	bool IsKeyPressed(EKey key);
	bool IsAnyKeyPressed();
	bool IsJoyBtnPressed(EJoy joyBtn);
	bool IsButtonPressed(EMouseButton btn);
	bool DoMouseMoved(vect2df_t* vDest = NULL);
	MouseEvent* GetButtonPressEvent(EMouseButton btn);
	bool GetTouch(vect2d_t* touch);
	vect2d_t getCurrInputPos();

#if TARGET_SDL || TARGET_SDL2
	void RegisterKeyEvent(uint32 eventType, SDL_Keysym key);
	void RegisterMouseEvent(uint32 eventType, vect2d_t mousePos, uint8 mouseBtn);
#endif
	void ProcessEvent(InputEvent* event);
	void FlushInputEvents();
};

#endif
