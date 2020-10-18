#ifndef IMAGE_HPP
#define IMAGE_HPP


#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>

#include "../core/constants.hpp"
#include "../core/common_types.hpp"
#include "../utils/math_tools.hpp"
#include "../utils/linked_list.hpp"
#include "../graphics/drawbuffer.hpp"


#define FREAD_BUFFER_SIZE   1000


class Image : public DrawBuffer {
protected:
	int m_paletteSize;
	color_t* m_aPalette;

public:
	Image(const char* fileName);
	~Image();

	void loadFromFile(const char* fileName);
    
	size2df_t getSize() { return m_size; }
	size2d_t getSizei() {
		size2d_t newSize;
		newSize.w = m_size.w;
		newSize.h = m_size.h;
		return newSize;
	}
};

#endif
