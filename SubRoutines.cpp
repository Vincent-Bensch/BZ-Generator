#include "Header.h"

double intdist;

vmath::vector::vector(double a, double b, double c) { i = a, j = b, k = c; } //Constructor for populating vector

void init()
{
	intdist = 0.0001;
	return;
}