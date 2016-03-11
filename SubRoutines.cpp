#include "Header.h"

//---------------------------------------------------Variables--------------------------------------------------
double intdist;

vmath::vector::vector(double a, double b, double c) { i = a, j = b, k = c; } //Constructor for populating vector
vmath::vector::vector() {} //Default constructor for vector

//--------------------------------------------------Sub Routines------------------------------------------------
void init() //Initializing global variables
{
	intdist = 0.0001;
	return;
}