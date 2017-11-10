#include "mathf.hpp"

int min(int a, int b) {
	return (a < b) ? a : b;
}

int max(int a, int b) {
	return (a >= b) ? a : b;
}

float minf(float a, float b) {
	return (a < b) ? a : b;
}

float maxf(float a, float b) {
	return (a >= b) ? a : b;
}

vect2df_t getNormalizedVect(vect2df_t vVect) {
	vect2df_t vNormalizedVect;

	if (abs(vVect.x) < abs(vVect.y)) {
		vNormalizedVect.x = vVect.x / abs(vVect.y);
		vNormalizedVect.y = vVect.y / abs(vVect.y);
	}
	else if (abs(vVect.x) > abs(vVect.y)) {
		vNormalizedVect.x = vVect.x / abs(vVect.x);
		vNormalizedVect.y = vVect.y / abs(vVect.x);
	}
	else {
		vNormalizedVect.x = vVect.x / abs(vVect.x);
		vNormalizedVect.y = vVect.y / abs(vVect.y);
	}

	return vNormalizedVect;
}

vect2df_t vectfSub(vect2df_t v1, vect2df_t v2) {
	vect2df_t vNewVect;
	vNewVect.x = v2.x - v1.x;
	vNewVect.y = v2.y - v1.y;

	return vNewVect;
}