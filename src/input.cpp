#include "input.hpp"


Input::Input() {
	
}

void Input::ScanInput() {
#ifdef TARGET_3DS
	hidScanInput();
#elif TARGET_WIN
	
#endif
}

uint32 Input::GetInput() {
#ifdef TARGET_3DS
	return hidKeysDown();

#else
	return 0x21212121;

#endif
}

bool Input::IsPressed(uint32 key) {
	uint32 kDown = GetInput();
	return (kDown & key);
}

bool Input::GetTouch(vect2d_t* touch) {
#ifdef TARGET_3DS
	touchPosition tp;
	hidTouchRead(&tp);

	if (tp.px + tp.py > 0) {
		vect2d_t touchPt;

		touch->x = tp.px;
		touch->y = tp.py;

		return true;
	}
	else {
		return false;
	}

#else
	touch->x = 0.;
	touch->y = 0.;

	return false;

#endif
}

void Input::RegisterKeyEvent(Uint32 eventType, SDL_Keysym key) {
	
}

void Input::RegisterMouseEvent(Uint32 eventType, vect2d_t mousePos) {
	
}
