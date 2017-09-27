#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

#ifdef TARGET_3DS

#define SCREEN_BPP		3
#define SCREEN_SCALE	1

#elif TARGET_SDL

#define SCREEN_BPP		4
#define SCREEN_SCALE	3

#endif