#pragma once
#ifndef HEADER_H_
#define HEADER_H_ //To make sharing subroutines and variables across .cpps easier

//----------------------------------------------Includes and Defines---------------------------------------------
#include <iostream> //For obvious reasons
#include <sstream> //For str( x )
#include <ctime> //For Timer

#define str( x ) dynamic_cast< std::ostringstream & >( \
	(std::ostringstream() << std::dec << x)).str() //No idea how this works, got it off stackexchange

using namespace std; //I'm lazy about typing std:: everywhere

//-----------------------------------------------External Variables---------------------------------------------
extern double intdist; //The closest distance I am willing to call an intersection(Not sure if this will come up but just in case)

//------------------------------------------------Function Headers----------------------------------------------
void init(); //Seting global variable values

//----------------------------------------------------Classes----------------------------------------------------
namespace vmath { //To avoid name conflicts, I put all my custom classes and variable types in the vmath namespace. Sort of negates my comment on using namespace std, but this is my code and I can be as inconsistant as I like!

	class timer {
	public:
		double a; //Using doubles as my numeric variable type in the entire program for precision consistancy
		double b; 
		void reset() { a = clock(); return; } //Restart stores current clock time
		double read() { return (clock() - a) / (double)CLOCKS_PER_SEC; } //Return ticks passed since last reset command and devide by ticks per second
	};

	class vector { //My vector class is for position and direction vectors. I am using position vectors instead of points because of the need for compatible classes in certian intersection algoritms
	public:
		double i, j, k; //The actual components of the vector

		void populate(double a, double b, double c) { i = a, j = b, k = c; return; } //So I don't have to use three lines every time I want to set a variable's values

		double magnitude() { return sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2)); } //Pythagrian theorem to get vector magnitude

		vector normalize() { //Return a unit vector pointing the same direction as orignial vector, by deviding all components by magnitude
			vector o;
			double l = magnitude();
			o.populate(
				i / l,
				j / l,
				k / l);
			return o;
		}

		double dot(vector v) { return i*v.i + j*v.j + k*v.k; } //a.dot(b) returns the dot product of a with b

		vector cross(vector v) { // a.cross(b) returnes the cross poduct of a with b
			vector o;
			o.populate(
				j*v.k - k*v.j,
				k*v.i - i*v.k,
				i*v.j - j*v.i);
			return o;
		}

		vector add(vector v) { //a.add(b) returns vector a plus vector b
			vector o;
			o.populate(
				i + v.i,
				j + v.j,
				k + v.k);
			return o;
		}

		vector subtract(vector v) { //a.subtract(b) returns vector a minus vector b
			vector o;
			o.populate(
				i - v.i,
				j - v.j,
				k - v.k);
			return o;
		}

		vector multiply(double d) { //a.multiply(b) returns vector a with all elements multiplied by double b
			vector o;
			o.populate(
				i*d,
				j*d,
				k*d);
			return o;
		}

		vector divide(double d) { //a.devide(b) returns vector a will all elements devided by double b
			vector o;
			o.populate(
				i / d,
				j / d,
				k / d);
			return o;
		}
	};

	class line { //Class for an enless line definded by a position and a direction vector
	public:
		vector direction; //Vector denoting direction of the line
		vector location; //Position vector marking a point on the line

		bool intersect(line i) { //Do the two lines intersect
			vector lcl = direction.cross(i.direction); 
			return abs(i.location.subtract(direction).dot(lcl) / lcl.magnitude()) <= intdist;
		} //No idea how the algoritm works, I am going to need to look into it

		vector intersection(line v) { //Where do two lines that intersect intersect
			return location.add(direction.multiply((direction.cross(v.direction).i) / (v.location.subtract(location).cross(v.direction).i)));
		} //No idea how the algoritm works, I am going to need to look into it
	};

	class plane { //Class for a plane defined by a position and a normal vector
	public:
		vector location; //Position vector marking a point on the plane
		vector normal; //Vector denoting the normal direction of the plane

		bool intersect(plane i) { return normal.dot(i.normal) == 1; } //If the normal vectors of two planes intersect, the planes intersect

		line intersection(plane i) { //Where do the planes intersect?
			line o;
			normal = normal.normalize();
			i.normal = i.normal.normalize();
			o.direction = normal.cross(i.normal);

			double h1 = normal.dot(location);
			double h2 = i.normal.dot(i.location);
			double p1 = normal.dot(i.normal);
			double p2 = 1 - pow(p1, 2);

			o.location = normal.multiply((h1 - h2*p1) / p2).add(i.normal.multiply((h2 - h1*p1) / p2));
			return o; //I have no idea how this works. I need to look into it.
		}
	};

	class linesegment { //Class for a linesegment defined by a start and an end point
	public:
		vector start, end; //Position vectors for the start and end points

		vector midpoint() { //Retrun a position vector marking the midpoint of the linesegment
			vector o;
			o.populate(
				(start.i + end.i) / 2,
				(start.j + end.j) / 2,
				(start.k + end.k) / 2);
			return o;
		}

		double length() { return sqrt(pow(end.i - start.i, 2) + pow(end.j - start.j, 2) + pow(end.k - start.k, 2)); } //Pythagrian theorem for the length of the linesegment
}

#endif