#ifdef TARGET_3DS
#include <3ds.h>
#else
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "common_types.hpp"


class Input {
public:
	Input();
	void ScanInput();
	uint32 GetInput();
	bool IsPressed(uint32 key);
	bool GetTouch(vect2d_t* touch);
};
