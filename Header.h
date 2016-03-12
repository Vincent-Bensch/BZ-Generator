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
	class linesegment; //Declaring linesegment class here so that plane class can access it before definition

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

		vector (double,double,double); //Function header for vector populating constructor
		vector(); //Function header for default constructor

		double magnitude() { return sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2)); } //Pythagrian theorem to get vector magnitude

		vector normalize() { //Return a unit vector pointing the same direction as orignial vector, by deviding all components by magnitude
			double l = magnitude();
			return vector(
				i / l,
				j / l,
				k / l);
		}

		double dot(vector in) { return i*in.i + j*in.j + k*in.k; } //a.dot(b) returns the dot product of a with b

		vector cross(vector in) { // a.cross(b) returnes the cross poduct of a with b
			return vector(
				j*in.k - k*in.j,
				k*in.i - i*in.k,
				i*in.j - j*in.i);
		}

		vector add(vector in) { //a.add(b) returns vector a plus vector b
			return vector(
				i + in.i,
				j + in.j,
				k + in.k);
		}

		vector subtract(vector in) { //a.subtract(b) returns vector a minus vector b
			return vector(
				i - in.i,
				j - in.j,
				k - in.k);
		}

		vector multiply(double d) { //a.multiply(b) returns vector a with all elements multiplied by double b
			return vector(
				i*d,
				j*d,
				k*d);
		}

		vector devide(double d) { //a.divide(b) returns vector a will all elements divided by double b
			return vector(
				i / d,
				j / d,
				k / d);
		}
	};

	class line { //Class for an enless line definded by a position and a direction vector
	public:
		vector dir; //Vector denoting direction of the line
		vector loc; //Position vector marking a point on the line

		line(vector, vector); //Function header for line populating
		line(); //Function header for default constructor

		bool intersect(line in) { //Determines if two lines intersect, see maths folder for details
			ta = (in.dir.i*(loc.j - in.loc.j) - in.dir.j*(loc.i - in.loc.i)) / (dir.i*in.dir.j - in.dir.i*dir.j);
			tb = (dir.i*(loc.j - in.loc.j) - dir.j*(loc.i - in.loc.i)) / (dir.i*in.dir.j - in.dir.i*dir.j);
			return ((loc.k + ta*dir.k) == (in.loc.k + tb*in.dir.k));
		}

		vector intersection(line in) { //Finds the intersection point for two lines, see maths folder for details
			return vector(
				loc.i + ta*dir.i,
				loc.j + ta*dir.j,
				loc.k + ta*dir.k);
		}

	private:
		static double ta; //These two variables are calculated to determine if ther is an intersection, but are also needed to calculate the intersection point
		static double tb;
	};

	class plane { //Class for a plane defined by a position and a normal vector 
	public:
		vector norm; //Vector denoting the normal direction of the plane
		vector loc; //Position vector marking a point on the plane
		double r; //R is used in Plane-Plane intersections

		plane(vector, vector); //Function header for plane populating and r calculating constructor
		plane(); //Function header for default constructor

		bool intersect(plane in) { return !(norm.dot(in.norm) == (norm.magnitude() * in.norm.magnitude())); } //If the normal vectos are paralell the planes do not intersect

		line intersection(plane in) { //Returns line of intersection between two lines see maths folder for details
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

		inline bool intersect(linesegment in); //Only declared here to avoid circular errors

	private:
		vector xintersection(plane in) { //Part of plane-plane intersection algorithem
			return vector(
				((in.norm.j*r - norm.j*in.r) / (norm.i*in.norm.j - in.norm.i*norm.j)),
				((in.norm.i*r - norm.i*in.r) / (norm.i*in.norm.j - in.norm.i*norm.j)),
				0);
		}

		vector yintersection(plane in) { //Part of plane-plane intersection algorithem
			return vector(
				((in.norm.k*r - norm.k*in.r) / (norm.i*in.norm.k - in.norm.i*norm.k)),
				0,
				((in.norm.i*r - norm.i*in.r) / (norm.i*in.norm.k - in.norm.i*norm.k)));
		}

		vector zintersection(plane in) { //Part of plane-plane intersection algorithem
			return vector(
				0,
				((in.norm.k*r - norm.k*in.r) / (norm.j*in.norm.k - in.norm.j*norm.k)),
				((in.norm.j*r - norm.j*in.r) / (norm.j*in.norm.k - in.norm.j*norm.k)));
		}
	};

	class linesegment { //Class for a linesegment defined by a start and an end point
	public:
		vector start, end; //Position vectors for the start and end points

		vector midpoint() { //Retrun a position vector marking the midpoint of the linesegment
			return vector(
				(start.i + end.i) / 2,
				(start.j + end.j) / 2,
				(start.k + end.k) / 2);
		}

		double length() { return sqrt(pow(end.i - start.i, 2) + pow(end.j - start.j, 2) + pow(end.k - start.k, 2)); } //Pythagrian theorem for the length of the linesegment
		
		bool intersect(plane in) { return (in.norm.dot(start.subtract(in.loc)) * in.norm.dot(end.subtract(in.loc))) <= 0; } //Tests for a intersection between a line segment and a plane. See maths folder for details
	};

	bool plane::intersect(linesegment in) { return (norm.dot(in.start.subtract(loc)) * norm.dot(in.end.subtract(loc))) <= 0; } //Tests for a intersection between a line segment and a plane. See maths folder for details
}

#endif