#include "input.hpp"


Input::Input() {
#ifdef TARGET_SDL2
    initKeyConvArray();
#elif TARGET_3DS
	initReverseJoyBtnConvArray();
#endif
	initList(&currEventsList);
	initList(&currInputList);
	initList(&currMouseList);
}

/*bool Input::IsPressed(uint32 key) {
	uint32 kDown = GetInput();
	return (kDown & key);
}*/

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

bool Input::IsAnyKeyPressed() {
	if (currInputList.size > 0)
		return true;
	else
		return false;
}

bool Input::IsJoyBtnPressed(EJoy joyBtn) {
#ifdef TARGET_3DS
	return hidKeysHeld() & (convertMuggineKeyTo3DS(joyBtn));
#else
	return false;
#endif
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

bool Input::DoMouseMoved(vect2df_t* vDest) {
	LLNode* currNode = currMouseList.pHead;
	
	while (currNode != NULL) {
		MouseEvent* currEvent = (MouseEvent*)currNode->pData;

		if (currEvent->type == MOUSE_EVT_MOTION) {
			printf("test");
			return true;
		}

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
	return false;

#endif
}

vect2d_t Input::getCurrInputPos() {
	return vCurrMousePos;
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

#ifdef TARGET_SDL2
void Input::RegisterKeyEvent(uint32 eventType, SDL_Keysym key) {
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

void Input::RegisterMouseEvent(uint32 eventType, vect2d_t mousePos, uint8 mouseBtn) {
	MouseEvent* mouseEvt = NULL;
	MouseEvent* currMouse = NULL;

	LLNode* currNode = currInputList.pHead;

	mousePos.x /= SCREEN_SCALE;
	mousePos.y /= SCREEN_SCALE;

	vCurrMousePos = mousePos;

	// printf("caca %d\n", mouseBtn);

	// mouseEvt = new MouseEvent;
	// mouseEvt->position = mousePos;

	if (eventType != SDL_MOUSEMOTION) {
		mouseEvt = new MouseEvent;
		mouseEvt->position = mousePos;

		if		(mouseBtn == 1) mouseEvt->btn = MOUSE_BTN_LEFT;
		else if (mouseBtn == 2) mouseEvt->btn = MOUSE_BTN_MIDDLE;
		else if (mouseBtn == 3) mouseEvt->btn = MOUSE_BTN_RIGHT;
		else					mouseEvt->btn = MOUSE_BTN_UNKNOWN;
	}
	else {
		// mouseEvt->type = MOUSE_EVT_MOTION;
	}

	if (eventType == SDL_MOUSEBUTTONDOWN) {
		mouseEvt->type = MOUSE_EVT_BTN_DOWN;

		if (IsButtonPressed(mouseEvt->btn)) {
			delete mouseEvt;
		}
		else {
			LLNode* newMouse = new LLNode;
			newMouse->pData = mouseEvt;
			addNodeToList(&currMouseList, newMouse);
		}
	}
	else if (eventType == SDL_MOUSEBUTTONUP) {
		mouseEvt->type = MOUSE_EVT_BTN_UP;

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
#endif

void Input::ProcessEvent(InputEvent* evt) {
	switch (evt->type) {
        case IN_KEYB: {
			KeyEvent* keyEvt = (KeyEvent*) evt->event;
            break;
        }
            
        default:
            break;
	}
}

void Input::FlushInputEvents() {

}
