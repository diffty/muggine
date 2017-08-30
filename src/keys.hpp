//
//  keys.hpp
//  Muggine
//
//  Created by DiFFtY on 07/03/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#ifndef KEYS_HPP
#define KEYS_HPP

#include "common_types.hpp"

#ifdef TARGET_SDL

#include <SDL2/SDL.h>

#elif TARGET_3DS

#include <3ds.h>

#endif

#include <iostream>


enum EKey {
    KEYB_UNKNOWN,
    KEYB_RETURN,
    KEYB_ESCAPE,
    KEYB_BACKSPACE,
    KEYB_TAB,
    KEYB_SPACE,
    KEYB_EXCLAIM,
    KEYB_QUOTEDBL,
    KEYB_HASH,
    KEYB_PERCENT,
    KEYB_DOLLAR,
    KEYB_AMPERSAND,
    KEYB_QUOTE,
    KEYB_LEFTPAREN,
    KEYB_RIGHTPAREN,
    KEYB_ASTERISK,
    KEYB_PLUS,
    KEYB_COMMA,
    KEYB_MINUS,
    KEYB_PERIOD,
    KEYB_SLASH,
    KEYB_0,
    KEYB_1,
    KEYB_2,
    KEYB_3,
    KEYB_4,
    KEYB_5,
    KEYB_6,
    KEYB_7,
    KEYB_8,
    KEYB_9,
    KEYB_COLON,
    KEYB_SEMICOLON,
    KEYB_LESS,
    KEYB_EQUALS,
    KEYB_GREATER,
    KEYB_QUESTION,
    KEYB_AT,
    KEYB_LEFTBRACKET,
    KEYB_BACKSLASH,
    KEYB_RIGHTBRACKET,
    KEYB_CARET,
    KEYB_UNDERSCORE,
    KEYB_BACKQUOTE,
    KEYB_A,
    KEYB_B,
    KEYB_C,
    KEYB_D,
    KEYB_E,
    KEYB_F,
    KEYB_G,
    KEYB_H,
    KEYB_I,
    KEYB_J,
    KEYB_K,
    KEYB_L,
    KEYB_M,
    KEYB_N,
    KEYB_O,
    KEYB_P,
    KEYB_Q,
    KEYB_R,
    KEYB_S,
    KEYB_T,
    KEYB_U,
    KEYB_V,
    KEYB_W,
    KEYB_X,
    KEYB_Y,
    KEYB_Z,
    KEYB_CAPSLOCK,
    KEYB_F1,
    KEYB_F2,
    KEYB_F3,
    KEYB_F4,
    KEYB_F5,
    KEYB_F6,
    KEYB_F7,
    KEYB_F8,
    KEYB_F9,
    KEYB_F10,
    KEYB_F11,
    KEYB_F12,
    KEYB_PRINTSCREEN,
    KEYB_SCROLLLOCK,
    KEYB_PAUSE,
    KEYB_INSERT,
    KEYB_HOME,
    KEYB_PAGEUP,
    KEYB_DELETE,
    KEYB_END,
    KEYB_PAGEDOWN,
    KEYB_RIGHT,
    KEYB_LEFT,
    KEYB_DOWN,
    KEYB_UP,
    KEYB_NUMLOCKCLEAR,
    KEYB_KP_DIVIDE,
    KEYB_KP_MULTIPLY,
    KEYB_KP_MINUS,
    KEYB_KP_PLUS,
    KEYB_KP_ENTER,
    KEYB_KP_1,
    KEYB_KP_2,
    KEYB_KP_3,
    KEYB_KP_4,
    KEYB_KP_5,
    KEYB_KP_6,
    KEYB_KP_7,
    KEYB_KP_8,
    KEYB_KP_9,
    KEYB_KP_0,
    KEYB_KP_PERIOD,
};

enum EJoy {
	JOY_BTN_1,
	JOY_BTN_2,
	JOY_BTN_3,
    JOY_BTN_4,
    JOY_BTN_START,
    JOY_BTN_SELECT,
    JOY_LEFT,
    JOY_RIGHT,
    JOY_UP,
    JOY_DOWN,
    JOY_DPAD_LEFT,
    JOY_DPAD_RIGHT,
    JOY_DPAD_UP,
    JOY_DPAD_DOWN
};

#ifdef TARGET_SDL
static EKey keyConvTab[256];
void initKeyConvArray();
EKey convertSDLToMuggineKey(SDL_Keycode keyCode);

#elif TARGET_3DS
static uint8 reverseJoyBtnConvTab[256];
void initReverseJoyBtnConvArray();
uint32 convertMuggineKeyTo3DS(EJoy);

#endif

#endif
