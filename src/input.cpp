#include "input.hpp"


Input::Input() {
#ifdef TARGET_SDL
    initKeyConvArray();
#endif
	initList(&currEventsList);
	initList(&currInputList);
	initList(&currMouseList);
}

void Input::ScanInput() {
#ifdef TARGET_3DS
	hidScanInput();

#elif TARGET_SDL
	
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

bool Input::IsKeyPressed(EKey key) {
	LLNode* currNode = currInputList.pHead;
	KeyEvent* currKey;

	while (currNode != NULL) {
		currKey = (KeyEvent*)currNode->pData;
		if (currKey->key == key) return true;
		currNode = currNode->pNext;
	}

	return false;
}

bool Input::IsButtonPressed(EMouseButton btn) {
	LLNode* currNode = currMouseList.pHead;
	MouseEvent* currMouseBtn;

	while (currNode != NULL) {
		currMouseBtn = (MouseEvent*)currNode->pData;
		if (currMouseBtn->btn == btn) return true;
		currNode = currNode->pNext;
	}

	return false;
}

MouseEvent* Input::GetButtonPressEvent(EMouseButton btn) {
	LLNode* currNode = currMouseList.pHead;
	MouseEvent* currMouseBtn;

	while (currNode != NULL) {
		currMouseBtn = (MouseEvent*)currNode->pData;
		if (currMouseBtn->btn == btn) return currMouseBtn;
		currNode = currNode->pNext;
	}

	return NULL;
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

void displayKeys(LinkedList* list) {
	LLNode* currNode = list->pHead;
	KeyEvent* currKey;

	while (currNode != NULL) {
		currKey = (KeyEvent*)currNode->pData;
		printf("%d ", currKey->key);
		currNode = currNode->pNext;
	}
	printf("\n");
}

void Input::RegisterKeyEvent(Uint32 eventType, SDL_Keysym key) {
    KeyEvent* keyEvt = new KeyEvent;
    keyEvt->key = convertSDLToMuggineKey(key.sym);

	//InputEvent* evt = new InputEvent;
	//evt->type = IN_KEYB;
	//evt->time = (float)time(NULL);
	//evt->event = &keyEvt;
    
	//LLNode* newInputEvt = new LLNode;
    //newInputEvt->pData = evt;
	//addNodeToList(&currEventsList, newInputEvt);

	KeyEvent* currKey = NULL;

	LLNode* currNode = currInputList.pHead;

	if (eventType == SDL_KEYDOWN) {
		//displayKeys(&currInputList);

		if (IsKeyPressed(keyEvt->key)) {
			delete keyEvt;
		}
		else {
			LLNode* newKey = new LLNode;
			newKey->pData = keyEvt;
			addNodeToList(&currInputList, newKey);
		}
	}
	else if (eventType == SDL_KEYUP) {
		//displayKeys(&currInputList);

		LLNode* prevNode = NULL;
		currNode = currInputList.pHead;

		while (currNode != NULL) {
			currKey = (KeyEvent*) currNode->pData;

			if (currKey->key == keyEvt->key) {
				LLNode* newCurrNode = currNode->pNext;

				if (prevNode != NULL) {
					prevNode->pNext = currNode->pNext;
				}
				else {
					currInputList.pHead = currNode->pNext;
				}

				if (currNode->pNext == NULL) {
					currInputList.pTail = prevNode;
				}

				delete currKey;
				delete currNode;

				currNode = newCurrNode;

				currInputList.size--;
			}
			else {
				prevNode = currNode;
				currNode = currNode->pNext;
			}
		}
	}
}

void Input::RegisterMouseEvent(Uint32 eventType, vect2d_t mousePos, Uint8 mouseBtn) {
	MouseEvent* mouseEvt = NULL;
	MouseEvent* currMouse = NULL;

	LLNode* currNode = currInputList.pHead;

	if (eventType != SDL_MOUSEMOTION) {
		mouseEvt = new MouseEvent;
		mouseEvt->position = mousePos;
		if		(mouseBtn == 1) mouseEvt->btn = MOUSE_BTN_LEFT;
		else if (mouseBtn == 2) mouseEvt->btn = MOUSE_BTN_MIDDLE;
		else if (mouseBtn == 3) mouseEvt->btn = MOUSE_BTN_RIGHT;
		else					mouseEvt->btn = MOUSE_BTN_UNKNOWN;
	}

	if (eventType == SDL_MOUSEBUTTONDOWN) {
		if (IsPressed(mouseEvt->btn)) {
			delete mouseEvt;
		}
		else {
			LLNode* newMouse = new LLNode;
			newMouse->pData = mouseEvt;
			addNodeToList(&currMouseList, newMouse);
		}
	}
	else if (eventType == SDL_MOUSEBUTTONUP) {
		LLNode* prevNode = NULL;
		currNode = currMouseList.pHead;

		while (currNode != NULL) {
			currMouse = (MouseEvent*) currNode->pData;

			if (currMouse->btn == mouseEvt->btn) {
				LLNode* newCurrNode = currNode->pNext;

				if (prevNode != NULL) {
					prevNode->pNext = currNode->pNext;
				}
				else {
					currMouseList.pHead = currNode->pNext;
				}

				if (currNode->pNext == NULL) {
					currMouseList.pTail = prevNode;
				}

				delete currMouse;
				delete currNode;

				currNode = newCurrNode;

				currMouseList.size--;
			}
			else {
				prevNode = currNode;
				currNode = currNode->pNext;
			}
		}
	}
}

void Input::ProcessEvent(InputEvent* evt) {
	switch (evt->type) {
		case IN_KEYB:
			KeyEvent* keyEvt = (KeyEvent*) evt->event;
		break;
	}
}

void Input::FlushInputEvents() {

}