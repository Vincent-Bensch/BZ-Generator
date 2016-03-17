#pragma once
#ifndef HEADER_H_
#define HEADER_H_ //To make sharing subroutines and variables across .cpps easier

//----------------------------------------------Includes and Defines---------------------------------------------
#include <iostream> //For obvious reasons
#include <sstream> //For str( x )
#include <fstream> //For file io
#include <ctime> //For Timer
#include <vector> //For lists
#include <algorithm> //For seek and destroy
#include <map> //For dictionaries

using namespace std; //I'm lazy about typing std:: everywhere

#define str( x ) dynamic_cast< ostringstream & >( \
	(ostringstream() << dec << x)).str() //No idea how this works, got it off stack exchange

//----------------------------------------------------Classes----------------------------------------------------
namespace vmath { //To avoid name conflicts, I put all my custom classes and variable types in the vmath namespace. Sort of negates my comment on using namespace std, but this is my code and I can be as inconsistent as I like!
	class linesegment; //Declaring line segment class here so that plane class can access it before definition

	class timer {
	private:
		double a; //Using doubles as my numeric variable type in the entire program for precision consistency
		double b;

	public:
		void reset() { a = clock(); return; } //Restart stores current clock time
		double read() { return (clock() - a) / (double)CLOCKS_PER_SEC; } //Return ticks passed since last reset command and divide by ticks per second
	};

	class vector { //My vector class is for position and direction vectors. I am using position vectors instead of points because of the need for compatible classes in certain intersection algorithms
	public:
		double i, j, k; //The actual components of the vector

		vector (double,double,double); //Function header for vector populating constructor
		vector(); //Function header for default constructor

		double magnitude() { return sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2)); } //Pythagorean theorem to get vector magnitude

		vector normalize() { //Return a unit vector pointing the same direction as original vector, by dividing all components by magnitude
			double l = magnitude();
			return vector(
				i / l,
				j / l,
				k / l);
		}

		double dot(vector in) { return i*in.i + j*in.j + k*in.k; } //a.dot(b) returns the dot product of a with b

		vector cross(vector in) { // a.cross(b) returns the cross product of a with b
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

		bool equals(vector in) { return (in.i == i && in.j == j && in.k == k); } //Compares all elements to determine if two vectors are identical
	};

	class line { //Class for an endless line defined by a position and a direction vector
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

		bool equals(line in) { return (dir.equals(in.dir) && loc.equals(in.loc)); } //Compares all elements to determine if two lines are identical
		
	private:
		double ta; //These two variables are calculated to determine if there is an intersection, but are also needed to calculate the intersection point
		double tb;
	};

	class plane { //Class for a plane defined by a position and a normal vector 
	public:
		vector norm; //Vector denoting the normal direction of the plane
		vector loc; //Position vector marking a point on the plane
		double r; //R is used in Plane-Plane intersections

		plane(vector, vector); //Function header for plane populating and r calculating constructor
		plane(); //Function header for default constructor

		bool intersect(plane in) { return !(norm.dot(in.norm) == (norm.magnitude() * in.norm.magnitude())); } //If the normal vectors are parallel the planes do not intersect

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

		bool equals(plane in) { return(norm.equals(in.norm) && loc.equals(in.loc)); } //Compares all elements to determine if two planes are identical

	private:
		vector xintersection(plane in) { //Part of plane-plane intersection algorithm
			return vector(
				((in.norm.j*r - norm.j*in.r) / (norm.i*in.norm.j - in.norm.i*norm.j)),
				((in.norm.i*r - norm.i*in.r) / (norm.i*in.norm.j - in.norm.i*norm.j)),
				0);
		}

		vector yintersection(plane in) { //Part of plane-plane intersection algorithm
			return vector(
				((in.norm.k*r - norm.k*in.r) / (norm.i*in.norm.k - in.norm.i*norm.k)),
				0,
				((in.norm.i*r - norm.i*in.r) / (norm.i*in.norm.k - in.norm.i*norm.k)));
		}

		vector zintersection(plane in) { //Part of plane-plane intersection algorithm
			return vector(
				0,
				((in.norm.k*r - norm.k*in.r) / (norm.j*in.norm.k - in.norm.j*norm.k)),
				((in.norm.j*r - norm.j*in.r) / (norm.j*in.norm.k - in.norm.j*norm.k)));
		}
	};

	class linesegment { //Class for a line segment defined by a start and an end point
	public:
		vector start, end; //Position vectors for the start and end points

		linesegment(vector, vector); //Function header for line segment populating constructor
		linesegment(); //Function header for default constructor

		vector midpoint() { //Return a position vector marking the midpoint of the line segment
			return vector(
				(start.i + end.i) / 2,
				(start.j + end.j) / 2,
				(start.k + end.k) / 2);
		}

		double length() { return sqrt(pow(end.i - start.i, 2) + pow(end.j - start.j, 2) + pow(end.k - start.k, 2)); } //Pythagorean theorem for the length of the line segment
		
		bool intersect(plane in) { return (in.norm.dot(start.subtract(in.loc)) * in.norm.dot(end.subtract(in.loc))) <= 0; } //Tests for a intersection between a line segment and a plane. See maths folder for details
	
		bool equals(linesegment in) { return((start.equals(in.start) && end.equals(in.end)) || (start.equals(in.end) && end.equals(in.start))); } //Compares all elements to determine if two line segments are identical
	};

	bool plane::intersect(linesegment in) { return (norm.dot(in.start.subtract(loc)) * norm.dot(in.end.subtract(loc))) <= 0; } //Tests for a intersection between a line segment and a plane. See maths folder for details
}

//----------------------------------------------Custom Variable Types--------------------------------------------
typedef vector<vmath::plane> planelist; //List of planes
typedef vector<vmath::line> linelist; //List of lines
typedef vector<vmath::linesegment> linesegmentlist; //List of line segments
typedef vector<vmath::vector> vectorlist; //List of vectors
typedef vector<linelist> linelistlist; //List of line lists, so they can be grouped by plane

//-----------------------------------------------External Variables---------------------------------------------
extern string logloc; //Sting holding the filename of the log file
extern string outloc; //String holding the base name for data output
extern vmath::vector origin; //Vector at 0,0,0

//--------------------------------------------------Utility Headers-----------------------------------------------
void init(); //Setting global variable values
string timestamp(); //Returns YYYY-MM-DD hh:mm:ss
void report(string in, bool file, bool screen); //Timestamps message and then posts it to screen and writes it to log file
void writepolygon(vectorlist in, int zone); //Write a polygon to a .obj file
int identifyzone(vmath::vector in, planelist planes); //Identify Brillouin zone a point belongs to
bool isorigin(vmath::vector in); //Is inputted vector origin?
vectorlist removeorigin(vectorlist in); //Remove instances origin from list of vectors

//-------------------------------------------------Brillouin Headers----------------------------------------------
vectorlist makePClattice(double max, double step); //Creates a Primitive Cube lattice
vectorlist makeBCClattice(double max, double step); //Creates a Body Centered Cubic lattice
vectorlist makeFCClattice(double max, double step); //Creates a Face Centered Cubic lattice
linesegmentlist makelinesegmentsfromlattice(vectorlist in); //Takes a list of line segments, and outputs a list of planes bisecting those line segments
planelist makebisectorplanes(linesegmentlist in); //Takes a list of line segments, and outputs a list of planes bisecting those line segments
linelistlist makelinesfromplanes(planelist in); //Takes a list of planes and returns the lines formed by intersection of those planes grouped by plane
vectorlist makepolygon(linesegmentlist in); //Takes a list of filtered but unsorted line-segments and organizes them into a sorted vectorlist that can be exported as a polygon

#endif