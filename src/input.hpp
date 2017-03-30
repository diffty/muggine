#ifndef INPUT_HPP
#define INPUT_HPP


#ifdef TARGET_3DS
#include <3ds.h>

#elif TARGET_SDL
#include <SDL2/SDL.h>

#else
#define _CRT_SECURE_NO_WARNINGS

#endif


#include <stdio.h>
#include <ctime>
#include "common_types.hpp"
#include "keys.hpp"
#include "linked_list.hpp"


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

struct KeyEvent {
    EKey key;
};

struct MouseEvent {
    EMouseButton btn;
    vect2d_t position;
};

struct InputEvent {
    int time;
    void* event;
	EInputType type;
};


class Input {
public:
	Input();
	void ScanInput();
	uint32 GetInput();
	bool IsPressed(uint32 key);
	bool IsKeyPressed(EKey key);
	bool IsJoyBtnPressed(EJoy joyBtn);
	bool IsButtonPressed(EMouseButton btn);
	MouseEvent* GetButtonPressEvent(EMouseButton btn);
	bool GetTouch(vect2d_t* touch);
	void RegisterKeyEvent(Uint32 eventType, SDL_Keysym key);
	void RegisterMouseEvent(Uint32 eventType, vect2d_t mousePos, Uint8 mouseBtn);
	void ProcessEvent(InputEvent* event);
	void FlushInputEvents();

private:
    LinkedList currEventsList;
	LinkedList currInputList;
	LinkedList currMouseList;
};

#endif
