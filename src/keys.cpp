#include "keys.hpp"


void initKeyConvArray() {
    keyConvTab[SDLK_UNKNOWN] = KEY_UNKNOWN;

    keyConvTab[SDLK_RETURN] = KEY_RETURN;
    keyConvTab[SDLK_ESCAPE] = KEY_ESCAPE;
    keyConvTab[SDLK_BACKSPACE] = KEY_BACKSPACE;
    keyConvTab[SDLK_TAB] = KEY_TAB;
    keyConvTab[SDLK_SPACE] = KEY_SPACE;
    keyConvTab[SDLK_EXCLAIM] = KEY_EXCLAIM;
    keyConvTab[SDLK_QUOTEDBL] = KEY_QUOTEDBL;
    keyConvTab[SDLK_HASH] = KEY_HASH;
    keyConvTab[SDLK_PERCENT] = KEY_PERCENT;
    keyConvTab[SDLK_DOLLAR] = KEY_DOLLAR;
    keyConvTab[SDLK_AMPERSAND] = KEY_AMPERSAND;
    keyConvTab[SDLK_QUOTE] = KEY_QUOTE;
    keyConvTab[SDLK_LEFTPAREN] = KEY_LEFTPAREN;
    keyConvTab[SDLK_RIGHTPAREN] = KEY_RIGHTPAREN;
    keyConvTab[SDLK_ASTERISK] = KEY_ASTERISK;
    keyConvTab[SDLK_PLUS] = KEY_PLUS;
    keyConvTab[SDLK_COMMA] = KEY_COMMA;
    keyConvTab[SDLK_MINUS] = KEY_MINUS;
    keyConvTab[SDLK_PERIOD] = KEY_PERIOD;
    keyConvTab[SDLK_SLASH] = KEY_SLASH;
    keyConvTab[SDLK_0] = KEY_0;
    keyConvTab[SDLK_1] = KEY_1;
    keyConvTab[SDLK_2] = KEY_2;
    keyConvTab[SDLK_3] = KEY_3;
    keyConvTab[SDLK_4] = KEY_4;
    keyConvTab[SDLK_5] = KEY_5;
    keyConvTab[SDLK_6] = KEY_6;
    keyConvTab[SDLK_7] = KEY_7;
    keyConvTab[SDLK_8] = KEY_8;
    keyConvTab[SDLK_9] = KEY_9;
    keyConvTab[SDLK_COLON] = KEY_COLON;
    keyConvTab[SDLK_SEMICOLON] = KEY_SEMICOLON;
    keyConvTab[SDLK_LESS] = KEY_LESS;
    keyConvTab[SDLK_EQUALS] = KEY_EQUALS;
    keyConvTab[SDLK_GREATER] = KEY_GREATER;
    keyConvTab[SDLK_QUESTION] = KEY_QUESTION;
    keyConvTab[SDLK_AT] = KEY_AT;

    keyConvTab[SDLK_LEFTBRACKET] = KEY_LEFTBRACKET;
    keyConvTab[SDLK_BACKSLASH] = KEY_BACKSLASH;
    keyConvTab[SDLK_RIGHTBRACKET] = KEY_RIGHTBRACKET;
    keyConvTab[SDLK_CARET] = KEY_CARET;
    keyConvTab[SDLK_UNDERSCORE] = KEY_UNDERSCORE;
    keyConvTab[SDLK_BACKQUOTE] = KEY_BACKQUOTE;
    keyConvTab[SDLK_a] = KEY_A;
    keyConvTab[SDLK_b] = KEY_B;
    keyConvTab[SDLK_c] = KEY_C;
    keyConvTab[SDLK_d] = KEY_D;
    keyConvTab[SDLK_e] = KEY_E;
    keyConvTab[SDLK_f] = KEY_F;
    keyConvTab[SDLK_g] = KEY_G;
    keyConvTab[SDLK_h] = KEY_H;
    keyConvTab[SDLK_i] = KEY_I;
    keyConvTab[SDLK_j] = KEY_J;
    keyConvTab[SDLK_k] = KEY_K;
    keyConvTab[SDLK_l] = KEY_L;
    keyConvTab[SDLK_m] = KEY_M;
    keyConvTab[SDLK_n] = KEY_N;
    keyConvTab[SDLK_o] = KEY_O;
    keyConvTab[SDLK_p] = KEY_P;
    keyConvTab[SDLK_q] = KEY_Q;
    keyConvTab[SDLK_r] = KEY_R;
    keyConvTab[SDLK_s] = KEY_S;
    keyConvTab[SDLK_t] = KEY_T;
    keyConvTab[SDLK_u] = KEY_U;
    keyConvTab[SDLK_v] = KEY_V;
    keyConvTab[SDLK_w] = KEY_W;
    keyConvTab[SDLK_x] = KEY_X;
    keyConvTab[SDLK_y] = KEY_Y;
    keyConvTab[SDLK_z] = KEY_Z;
}

EKey convertSDLToMuggineKey(SDL_Keycode keyCode) {
    return keyConvTab[keyCode];
}