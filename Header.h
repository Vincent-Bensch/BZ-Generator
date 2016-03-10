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
	private:
		double a; //Using doubles as my numeric variable type in the entire program for precision consistancy
		double b;

	public:
		void reset() { a = clock(); return; } //Restart stores current clock time
		double read() { return (clock() - a) / (double)CLOCKS_PER_SEC; } //Return ticks passed since last reset command and devide by ticks per second
	};

	class vector { //My vector class is for position and direction vectors. I am using position vectors instead of points because of the need for compatible classes in certian intersection algoritms
	public:
		double i, j, k; //The actual components of the vector

		vector (double,double,double); //Function header for vector populating

		double magnitude() { return sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2)); } //Pythagrian theorem to get vector magnitude

		vector normalize() { //Return a unit vector pointing the same direction as orignial vector, by deviding all components by magnitude
			double l = magnitude();
			vector out(
				i / l,
				j / l,
				k / l);
			return out;
		}

		double dot(vector in) { return i*in.i + j*in.j + k*in.k; } //a.dot(b) returns the dot product of a with b

		vector cross(vector in) { // a.cross(b) returnes the cross poduct of a with b
			vector out(
				j*in.k - k*in.j,
				k*in.i - i*in.k,
				i*in.j - j*in.i);
			return out;
		}

		vector add(vector in) { //a.add(b) returns vector a plus vector b
			vector out(
				i + in.i,
				j + in.j,
				k + in.k);
			return out;
		}

		vector subtract(vector in) { //a.subtract(b) returns vector a minus vector b
			vector out(
				i - in.i,
				j - in.j,
				k - in.k);
			return out;
		}

		vector multiply(double d) { //a.multiply(b) returns vector a with all elements multiplied by double b
			vector out(
				i*d,
				j*d,
				k*d);
			return out;
		}

		vector divide(double d) { //a.devide(b) returns vector a will all elements devided by double b
			vector out(
				i / d,
				j / d,
				k / d);
			return out;
		}
	};


	class line { //Class for an enless line definded by a position and a direction vector
	public:
		vector dir; //Vector denoting direction of the line
		vector loc; //Position vector marking a point on the line

	};

	class plane { //Class for a plane defined by a position and a normal vector
	public:
		vector norm; //Vector denoting the normal direction of the plane
		vector loc; //Position vector marking a point on the plane

	};

	class linesegment { //Class for a linesegment defined by a start and an end point
	public:
		vector a, b; //Position vectors for the start and end points

		vector midpoint() { //Retrun a position vector marking the midpoint of the linesegment
			vector out(
				(a.i + b.i) / 2,
				(a.j + b.j) / 2,
				(a.k + b.k) / 2);
			return out;
		}

		double length() { return sqrt(pow(b.i - a.i, 2) + pow(b.j - a.j, 2) + pow(b.k - a.k, 2)); } //Pythagrian theorem for the length of the linesegment
	};
}

#endif