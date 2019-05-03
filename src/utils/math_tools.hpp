#ifndef MATH_TOOLS_HPP
#define MATH_TOOLS_HPP

#include "../core/common_types.hpp"
#include <cmath>

vect2df_t getNormalizedVect(vect2df_t vVect);
float lerpf(float a, float b, float coef);
int minInt(int a, int b);
int maxInt(int a, int b);
float minf(float a, float b);
float maxf(float a, float b);
vect2df_t getNormalizedVect(vect2df_t vVect);
vect2df_t vectfSub(vect2df_t v1, vect2df_t v2);

#endif
