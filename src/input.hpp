#ifdef TARGET_3DS
#include <3ds.h>
#else
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "common_types.hpp"
#include "system.hpp"


class Input {
public:
	Input();
	Input(System* sys);
	void ScanInput();
	uint32 GetInput();
	bool IsPressed(uint32 key);
	bool GetTouch(vect2d_t* touch);

private:
	System* m_sys;
};
