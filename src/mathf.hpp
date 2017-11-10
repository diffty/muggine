#ifndef MATHF_HPP
#define MATHF_HPP


#include <cmath>
#include "common_types.hpp"


int min(int a, int b);
int max(int a, int b);

float minf(float a, float b);
float maxf(float a, float b);

vect2df_t getNormalizedVect(vect2df_t vVect);
vect2df_t vectfSub(vect2df_t v1, vect2df_t v2);

#endif