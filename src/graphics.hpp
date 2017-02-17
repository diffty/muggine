#ifdef TARGET_3DS
#include <3ds.h>
#endif

#include "common_types.hpp"


class Graphics {
public:
	Graphics();
	void Init();
	void SetDoubleBuffering(bool isActive);
	uint8* GetFramebuffer();
	void FlushBuffer();
	void SwapBuffer();
	void WaitForBlank();
	void Exit();
};