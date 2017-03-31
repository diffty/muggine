#include "keys.hpp"



#ifdef TARGET_SDL
void initKeyConvArray() {
    keyConvTab[SDLK_UNKNOWN] = KEYB_UNKNOWN;

    keyConvTab[SDLK_RETURN] = KEYB_RETURN;
    keyConvTab[SDLK_ESCAPE] = KEYB_ESCAPE;
    keyConvTab[SDLK_BACKSPACE] = KEYB_BACKSPACE;
    keyConvTab[SDLK_TAB] = KEYB_TAB;
    keyConvTab[SDLK_SPACE] = KEYB_SPACE;
    keyConvTab[SDLK_EXCLAIM] = KEYB_EXCLAIM;
    keyConvTab[SDLK_QUOTEDBL] = KEYB_QUOTEDBL;
    keyConvTab[SDLK_HASH] = KEYB_HASH;
    keyConvTab[SDLK_PERCENT] = KEYB_PERCENT;
    keyConvTab[SDLK_DOLLAR] = KEYB_DOLLAR;
    keyConvTab[SDLK_AMPERSAND] = KEYB_AMPERSAND;
    keyConvTab[SDLK_QUOTE] = KEYB_QUOTE;
    keyConvTab[SDLK_LEFTPAREN] = KEYB_LEFTPAREN;
    keyConvTab[SDLK_RIGHTPAREN] = KEYB_RIGHTPAREN;
    keyConvTab[SDLK_ASTERISK] = KEYB_ASTERISK;
    keyConvTab[SDLK_PLUS] = KEYB_PLUS;
    keyConvTab[SDLK_COMMA] = KEYB_COMMA;
    keyConvTab[SDLK_MINUS] = KEYB_MINUS;
    keyConvTab[SDLK_PERIOD] = KEYB_PERIOD;
    keyConvTab[SDLK_SLASH] = KEYB_SLASH;
    keyConvTab[SDLK_0] = KEYB_0;
    keyConvTab[SDLK_1] = KEYB_1;
    keyConvTab[SDLK_2] = KEYB_2;
    keyConvTab[SDLK_3] = KEYB_3;
    keyConvTab[SDLK_4] = KEYB_4;
    keyConvTab[SDLK_5] = KEYB_5;
    keyConvTab[SDLK_6] = KEYB_6;
    keyConvTab[SDLK_7] = KEYB_7;
    keyConvTab[SDLK_8] = KEYB_8;
    keyConvTab[SDLK_9] = KEYB_9;
    keyConvTab[SDLK_COLON] = KEYB_COLON;
    keyConvTab[SDLK_SEMICOLON] = KEYB_SEMICOLON;
    keyConvTab[SDLK_LESS] = KEYB_LESS;
    keyConvTab[SDLK_EQUALS] = KEYB_EQUALS;
    keyConvTab[SDLK_GREATER] = KEYB_GREATER;
    keyConvTab[SDLK_QUESTION] = KEYB_QUESTION;
    keyConvTab[SDLK_AT] = KEYB_AT;

    keyConvTab[SDLK_LEFTBRACKET] = KEYB_LEFTBRACKET;
    keyConvTab[SDLK_BACKSLASH] = KEYB_BACKSLASH;
    keyConvTab[SDLK_RIGHTBRACKET] = KEYB_RIGHTBRACKET;
    keyConvTab[SDLK_CARET] = KEYB_CARET;
    keyConvTab[SDLK_UNDERSCORE] = KEYB_UNDERSCORE;
    keyConvTab[SDLK_BACKQUOTE] = KEYB_BACKQUOTE;
    keyConvTab[SDLK_a] = KEYB_A;
    keyConvTab[SDLK_b] = KEYB_B;
    keyConvTab[SDLK_c] = KEYB_C;
    keyConvTab[SDLK_d] = KEYB_D;
    keyConvTab[SDLK_e] = KEYB_E;
    keyConvTab[SDLK_f] = KEYB_F;
    keyConvTab[SDLK_g] = KEYB_G;
    keyConvTab[SDLK_h] = KEYB_H;
    keyConvTab[SDLK_i] = KEYB_I;
    keyConvTab[SDLK_j] = KEYB_J;
    keyConvTab[SDLK_k] = KEYB_K;
    keyConvTab[SDLK_l] = KEYB_L;
    keyConvTab[SDLK_m] = KEYB_M;
    keyConvTab[SDLK_n] = KEYB_N;
    keyConvTab[SDLK_o] = KEYB_O;
    keyConvTab[SDLK_p] = KEYB_P;
    keyConvTab[SDLK_q] = KEYB_Q;
    keyConvTab[SDLK_r] = KEYB_R;
    keyConvTab[SDLK_s] = KEYB_S;
    keyConvTab[SDLK_t] = KEYB_T;
    keyConvTab[SDLK_u] = KEYB_U;
    keyConvTab[SDLK_v] = KEYB_V;
    keyConvTab[SDLK_w] = KEYB_W;
    keyConvTab[SDLK_x] = KEYB_X;
    keyConvTab[SDLK_y] = KEYB_Y;
    keyConvTab[SDLK_z] = KEYB_Z;
}

EKey convertSDLToMuggineKey(SDL_Keycode keyCode) {
	return keyConvTab[keyCode];
}

#elif TARGET_3DS
void initReverseJoyBtnConvArray() {
	reverseJoyBtnConvTab[JOY_BTN_1] = KEY_A;
	reverseJoyBtnConvTab[JOY_BTN_2] = KEY_B;
	reverseJoyBtnConvTab[JOY_BTN_3] = KEY_X;
	reverseJoyBtnConvTab[JOY_BTN_4] = KEY_Y;
}

uint32 convertMuggineKeyTo3DS(EJoy joyKey) {
	return reverseJoyBtnConvTab[joyKey];
}
#endif
