#include "Header.h"

//---------------------------------------------------Variables--------------------------------------------------
double intdist;

//--------------------------------------------------Constructors------------------------------------------------
vmath::vector::vector(double a, double b, double c) { i = a, j = b, k = c; } //Constructor for populating vector
vmath::vector::vector() {} //Default constructor for vector

vmath::plane::plane(vector a, vector b) { norm = a, loc = b, r = norm.dot(loc); } //Constructor for populating plane and calculating r
vmath::plane::plane() {} //Default constructor for plane

//--------------------------------------------------Sub Routines------------------------------------------------
void init() //Initializing global variables
{
	intdist = 0.0001;
	return;
}