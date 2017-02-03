#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <3ds.h>


typedef struct vect2d_t {
	long x;
	long y;
} vect2d;

typedef struct size2d_t {
	long w;
	long h;
} size2d;

typedef struct color_t {
	unsigned int r;
	unsigned int g;
	unsigned int b;
} color_t;



class Color {
private:
	color_t m_color;
	u8* m_framebuffer3DSFmt;

	u8* Update3DSFramebufferFormat() {
		m_framebuffer3DSFmt[0] = (u8) m_color.b;
		m_framebuffer3DSFmt[1] = (u8) m_color.g;
		m_framebuffer3DSFmt[2] = (u8) m_color.r;
	}

public:
	Color(unsigned int r, unsigned int g, unsigned int b) {
		m_framebuffer3DSFmt = new u8[3];
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

	u8* get3DSFramebufferFormat() {
		return m_framebuffer3DSFmt;
	}
};


class Rect {
private:
	vect2d m_pos;
	size2d m_size;

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

	vect2d_t getPos()			{ return m_pos; }
	size2d_t getSize()			{ return m_size; }
	bool setPos(int x, int y)	{ m_pos.x = x; m_pos.y = y; }
	bool setSize(int w, int h)	{ m_size.w = w; m_size.h = h; }
};


#endif