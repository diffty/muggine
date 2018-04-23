#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#ifdef TARGET_3DS
#include <3ds.h>
#endif


// Platform-specific type defines
#ifdef TARGET_3DS
typedef u8 uint8;
typedef u32 uint32;
#else
#include <cstdint>
typedef uint8_t uint8;
typedef uint32_t uint32;
#endif



typedef struct vect2d_t {
	long x;
	long y;
} vect2d_t;

typedef struct vect2df_t {
	float x;
	float y;
} vect2df_t;

typedef struct size2d_t {
	long w;
	long h;
} size2d_t;

typedef struct size2df_t {
	long w;
	long h;
} size2df_t;

typedef struct color_t {
	unsigned int r;
	unsigned int g;
	unsigned int b;
} color_t;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;


class Color {
private:
	color_t m_color;
	uint8* m_framebuffer3DSFmt;

	void Update3DSFramebufferFormat() {
		m_framebuffer3DSFmt[0] = (uint8)m_color.b;
		m_framebuffer3DSFmt[1] = (uint8)m_color.g;
		m_framebuffer3DSFmt[2] = (uint8)m_color.r;
	}

public:
	Color(unsigned int r = 0, unsigned int g = 0, unsigned int b = 0) {
		m_framebuffer3DSFmt = new uint8[3];
		setColor(r, g, b);
	}

	~Color() {
		delete m_framebuffer3DSFmt;
	}

	color_t getColor() {
		return m_color;
	}

	void setColor(unsigned int r, unsigned int g, unsigned int b) {
		m_color.r = r;
		m_color.g = g;
		m_color.b = b;

		Update3DSFramebufferFormat();
	}

	uint8* get3DSFramebufferFormat() {
		return m_framebuffer3DSFmt;
	}
};


class Rect {
private:
	vect2d_t m_pos;
	size2d_t m_size;

public:
	Rect(int x, int y, int w, int h) {
		m_pos.x = x;
		m_pos.y = y;
		m_size.w = w;
		m_size.h = h;
	}

	bool isPointInRect(int x, int y) {
		if (m_pos.x <= x && m_pos.y <= y && m_pos.x + m_size.w >= x && m_pos.y + m_size.h >= y)
			return true;
		else
			return false;
	}

	vect2d_t getPos() { return m_pos; }
	size2d_t getSize() { return m_size; }
	void setPos(int x, int y) { m_pos.x = x; m_pos.y = y; }
	void setSize(int w, int h) { m_size.w = w; m_size.h = h; }
};


class Rectf {
private:
	vect2df_t m_pos;
	size2df_t m_size;

public:
	Rectf(float x, float y, float w, float h) {
		m_pos.x = x;
		m_pos.y = y;
		m_size.w = w;
		m_size.h = h;
	}

	bool isPointInRect(float x, float y) {
		if (m_pos.x <= x
			&& m_pos.y <= y
			&& m_pos.x + m_size.w >= x
			&& m_pos.y + m_size.h >= y)
			return true;
		else
			return false;
	}
    
    /*bool isRectIntersect(Rectf otherRect) {
        
    }*/

	vect2df_t getPos()  { return m_pos; }
	size2df_t getSize() { return m_size; }
	void setPos(float x, float y) { m_pos.x = x; m_pos.y = y; }
	void setSize(float w, float h) { m_size.w = w; m_size.h = h; }

	vect2d_t getPosi() {
		vect2d_t newPos;
		newPos.x = m_pos.x;
		newPos.y = m_pos.y;
		return newPos;
	}

};


#endif
