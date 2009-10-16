#ifndef IMPROVED_NOISE_

#define IMPROVED_NOISE_

#include <stdlib.h>
#include <math.h>

class ImprovedNoise {

public:
	ImprovedNoise();

	double noise(double x, double y, double z);

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	//int * p;
	int p[512];
};

#endif
