#include "Header.h"

//--------------------------------------------------Constructors------------------------------------------------
vmath::vector::vector(double a, double b, double c) { i = a, j = b, k = c; } //Constructor for populating vector
vmath::vector::vector() {} //Default constructor for vector

vmath::plane::plane(vector a, vector b) { norm = a, loc = b, r = norm.dot(loc); } //Constructor for populating plane and calculating r
vmath::plane::plane() {} //Default constructor for plane

vmath::line::line(vector a, vector b) { dir = a, loc = b; } //Constructor for populating line
vmath::line::line() {} //Default constructor for line

vmath::linesegment::linesegment(vector a, vector b) { start = a, end = b; } //Function header for line segment populating constructor
vmath::linesegment::linesegment() {} //Function header for default constructor

namespace vmath { //To avoid name conflicts, I put all my custom classes in the vmath namespace. Sort of negates my comment on using namespace std, but this is my code and I can be as inconsistent as I like!

	//------------------------------------------------------Timer----------------------------------------------------
	void timer::reset() { a = clock(); return; } //Restart stores current clock time
	
	double timer::read() { return (clock() - a) / (double)CLOCKS_PER_SEC; } //Return ticks passed since last reset command and divide by ticks per second

	//------------------------------------------------------Vector---------------------------------------------------
	double vector::magnitude() { return sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2)); } //Pythagorean theorem to get vector magnitude

	vector vector::normalize() { //Return a unit vector pointing the same direction as original vector, by dividing all components by magnitude
		double l = magnitude();
		return vector(
			i / l,
			j / l,
			k / l);
	}

	double vector::dot(vector in) { return i*in.i + j*in.j + k*in.k; } //a.dot(b) returns the dot product of a with b

	vector vector::cross(vector in) { // a.cross(b) returns the cross product of a with b
		return vector(
			j*in.k - k*in.j,
			k*in.i - i*in.k,
			i*in.j - j*in.i);
	}

	vector vector::add(vector in) { //a.add(b) returns vector a plus vector b
		return vector(
			i + in.i,
			j + in.j,
			k + in.k);
	}

	vector vector::subtract(vector in) { //a.subtract(b) returns vector a minus vector b
		return vector(
			i - in.i,
			j - in.j,
			k - in.k);
	}

	vector vector::multiply(double d) { //a.multiply(b) returns vector a with all elements multiplied by double b
		return vector(
			i*d,
			j*d,
			k*d);
	}

	vector vector::divide(double d) { //a.divide(b) returns vector a will all elements divided by double b
		return vector(
			i / d,
			j / d,
			k / d);
	}

	bool vector::equals(vector in) { return (in.i == i && in.j == j && in.k == k); } //Compares all elements to determine if two vectors are identical

	//-------------------------------------------------------Line----------------------------------------------------
	bool line::intersect(line in) { //Determines if two lines intersect, see maths folder for details
		ta = (in.dir.i*(loc.j - in.loc.j) - in.dir.j*(loc.i - in.loc.i)) / (dir.i*in.dir.j - in.dir.i*dir.j);
		tb = (dir.i*(loc.j - in.loc.j) - dir.j*(loc.i - in.loc.i)) / (dir.i*in.dir.j - in.dir.i*dir.j);
		return ((loc.k + ta*dir.k) == (in.loc.k + tb*in.dir.k));
	}

	vector line::intersection(line in) { //Finds the intersection point for two lines, see maths folder for details
		return vector(
			loc.i + ta*dir.i,
			loc.j + ta*dir.j,
			loc.k + ta*dir.k);
	}

	bool line::equals(line in) { return (dir.equals(in.dir) && loc.equals(in.loc)); } //Compares all elements to determine if two lines are identical

	//-------------------------------------------------------Plane---------------------------------------------------

	bool plane::intersect(plane in) { return !(norm.dot(in.norm) == (norm.magnitude() * in.norm.magnitude())); } //If the normal vectors are parallel the planes do not intersect

	line plane::intersection(plane in) { //Returns line of intersection between two lines see maths folder for details
		if (norm.i != 0 && norm.j != 0 && in.norm.i != 0 && in.norm.j != 0) {
			return line(norm.cross(in.norm), xintersection(in));
		}

		if (norm.i != 0 && norm.k != 0 && in.norm.i != 0 && in.norm.k != 0) {
			return line(norm.cross(in.norm), yintersection(in));
		}

		if (norm.j != 0 && norm.k != 0 && in.norm.j != 0 && in.norm.k != 0) {
			return line(norm.cross(in.norm), zintersection(in));
		}
	}

	bool plane::equals(plane in) { return(norm.equals(in.norm) && loc.equals(in.loc)); } //Compares all elements to determine if two planes are identical

	vector plane::xintersection(plane in) { //Part of plane-plane intersection algorithm
		return vector(
			((in.norm.j*r - norm.j*in.r) / (norm.i*in.norm.j - in.norm.i*norm.j)),
			((in.norm.i*r - norm.i*in.r) / (norm.i*in.norm.j - in.norm.i*norm.j)),
			0);
	}

	vector plane::yintersection(plane in) { //Part of plane-plane intersection algorithm
		return vector(
			((in.norm.k*r - norm.k*in.r) / (norm.i*in.norm.k - in.norm.i*norm.k)),
			0,
			((in.norm.i*r - norm.i*in.r) / (norm.i*in.norm.k - in.norm.i*norm.k)));
	}

	vector plane::zintersection(plane in) { //Part of plane-plane intersection algorithm
		return vector(
			0,
			((in.norm.k*r - norm.k*in.r) / (norm.j*in.norm.k - in.norm.j*norm.k)),
			((in.norm.j*r - norm.j*in.r) / (norm.j*in.norm.k - in.norm.j*norm.k)));
	}

	//--------------------------------------------------Line Segment-------------------------------------------------
	vector linesegment::midpoint() { //Return a position vector marking the midpoint of the line segment
		return vector(
			(start.i + end.i) / 2,
			(start.j + end.j) / 2,
			(start.k + end.k) / 2);
	}

	double linesegment::length() { return sqrt(pow(end.i - start.i, 2) + pow(end.j - start.j, 2) + pow(end.k - start.k, 2)); } //Pythagorean theorem for the length of the line segment

	bool linesegment::intersect(plane in) { return (in.norm.dot(start.subtract(in.loc)) * in.norm.dot(end.subtract(in.loc))) <= 0; } //Tests for a intersection between a line segment and a plane. See maths folder for details

	bool linesegment::equals(linesegment in) { return((start.equals(in.start) && end.equals(in.end)) || (start.equals(in.end) && end.equals(in.start))); } //Compares all elements to determine if two line segments are identical
}