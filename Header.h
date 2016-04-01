#pragma once
#ifndef HEADER_H_
#define HEADER_H_ //To make sharing subroutines and variables across .cpps easier

//----------------------------------------------Includes and Defines---------------------------------------------
#include <iostream> //For obvious reasons
#include <sstream> //For str( x )
#include <fstream> //For file io
#include <windows.h> //For creating directories
#include <ctime> //For Timer
#include <vector> //For lists
#include <algorithm> //For seek and destroy
#include <map> //For dictionaries
#include <cstdlib> //For crashing in failstate
#include <string> //For space multiplication

using namespace std; //I'm lazy about typing std:: everywhere

#define str( x ) dynamic_cast< ostringstream & >( \
	(ostringstream() << dec << x)).str() //No idea how this works, got it off stack exchange

//-------------------------------------------------Vmath Headers-------------------------------------------------
namespace vmath { //To avoid name conflicts, I put all my custom classes in the vmath namespace. Sort of negates my comment on using namespace std, but this is my code and I can be as inconsistent as I like!
	class timer;
	class vector;
	class line;
	class plane;
	class linesegment;
	class configuration;

	class timer {
	private:
		double a; //Using doubles as my numeric variable type in the entire program for precision consistency
		double b;

	public:
		void reset(); //Restart stores current clock time
		double read(); //Return ticks passed since last reset command and divide by ticks per second
	};

	class vector { //My vector class is for position and direction vectors. I am using position vectors instead of points because of the need for compatible classes in certain intersection algorithms
	public:
		double i, j, k; //The actual components of the vector

		vector(double, double, double); //Function header for vector populating constructor
		vector(); //Function header for default constructor

		double magnitude(); //Pythagorean theorem to get vector magnitude
		vector normalize(); //Return a unit vector pointing the same direction as original vector, by dividing all components by magnitude
		double dot(vector in); //a.dot(b) returns the dot product of a with b
		vector cross(vector in); // a.cross(b) returns the cross product of a with b
		vector add(vector in); //a.add(b) returns vector a plus vector b
		vector subtract(vector in); //a.subtract(b) returns vector a minus vector b
		vector multiply(double d); //a.multiply(b) returns vector a with all elements multiplied by double b
		vector divide(double d); //a.divide(b) returns vector a will all elements divided by double d
		bool equals(vector in); //Compares all elements to determine if two vectors are identical
	};

	class line { //Class for an endless line defined by a position and a direction vector
	public:
		vector dir; //Vector denoting direction of the line
		vector loc; //Position vector marking a point on the line

		line(vector, vector); //Function header for line populating
		line(); //Function header for default constructor

		bool intersect(line in); //Determines if two lines intersect, see maths folder for details
		vector intersection(line in); //Finds the intersection point for two lines, see maths folder for details
		bool equals(line in); //Compares all elements to determine if two lines are identical

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

		bool intersect(plane in); //If the normal vectors are parallel the planes do not intersect
		line intersection(plane in); //Returns line of intersection between two lines see maths folder for details
		bool intersect(linesegment in); //Tests for a intersection between a line segment and a plane. See maths folder for details
		bool equals(plane in); //Compares all elements to determine if two planes are identical

	private:
		vector xintersection(plane in); //Part of plane-plane intersection algorithm
		vector yintersection(plane in); //Part of plane-plane intersection algorithm
		vector zintersection(plane in); //Part of plane-plane intersection algorithm
	};

	class linesegment { //Class for a line segment defined by a start and an end point
	public:
		vector start, end; //Position vectors for the start and end points

		linesegment(vector, vector); //Function header for line segment populating constructor
		linesegment(); //Function header for default constructor

		vector midpoint(); //Return a position vector marking the midpoint of the line segment
		double length(); //Pythagorean theorem for the length of the line segment
		bool intersect(plane in); //Tests for a intersection between a line segment and a plane. See maths folder for details
		bool equals(linesegment in); //Compares all elements to determine if two line segments are identical
	};

	class configuration {
	public:
		string latticetype_s;
		int latticetype_i;
		double maxlatticedistance;
		double latticestep;
		int maximumzone;

		configuration();
		configuration(int, double, double, int, string);

		void full_display();
		void run();
		void write();
	};
}

//----------------------------------------------Custom Variable Types--------------------------------------------
typedef vector<vmath::plane> planelist; //List of planes
typedef vector<vmath::line> linelist; //List of lines
typedef vector<vmath::linesegment> linesegmentlist; //List of line segments
typedef vector<vmath::vector> vectorlist; //List of vectors
typedef vector<linelist> linelistlist; //List of line lists, so they can be grouped by plane
typedef vector<vmath::configuration> configurationlist; //List of configurations
typedef vector<string> stringlist; //List of strings

//-------------------------------------------------External Variables---------------------------------------------
extern string folder; //The folder into which the output gets pushed
extern string logloc; //Sting holding the filename of the log file
extern string outloc; //String holding the base name for data output
extern string configloc; //String holding the name of the configuration file
extern int dispspacing;
extern vmath::vector origin; //Vector at 0,0,0

//--------------------------------------------------Utility Headers-----------------------------------------------
void init(); //Setting global variable values
string spaceout(stringlist in);
string twodigit(int in); //Takes a one or two digit integer and returns a sting of the int. If the int only has one digit, a 0 is added to compensate
string timestamp(bool file = false); //Create a timestamp either for a filename or log content
void report(string in, bool file = true, bool screen = true); //Timestamps message and then posts it to screen and writes it to log file
void writepolygon(vectorlist in, int zone); //Write a polygon to a .obj file
int identifyzone(vmath::vector in, planelist planes); //Identify Brillouin zone a point belongs to
bool isorigin(vmath::vector in); //Is inputted vector origin?
vectorlist removeorigin(vectorlist in); //Remove instances origin from list of vectors
string requeststring(string ask);  //Prints ask string and then returns user input as a string
int requestint(string ask);  //Prints ask string and then returns user input as a int
double requestdouble(string ask);  //Prints ask string and then returns user input as a double

//-------------------------------------------------Brillouin Headers----------------------------------------------
vectorlist makePClattice(double max, double step); //Creates a Primitive Cube lattice
vectorlist makeBCClattice(double max, double step); //Creates a Body Centered Cubic lattice
vectorlist makeFCClattice(double max, double step); //Creates a Face Centered Cubic lattice
linesegmentlist makelinesegmentsfromlattice(vectorlist in); //Takes a list of line segments, and outputs a list of planes bisecting those line segments
planelist makebisectorplanes(linesegmentlist in); //Takes a list of line segments, and outputs a list of planes bisecting those line segments
linelistlist makelinesfromplanes(planelist in); //Takes a list of planes and returns the lines formed by intersection of those planes grouped by plane
vectorlist makepolygon(linesegmentlist in); //Takes a list of filtered but unsorted line-segments and organizes them into a sorted vectorlist that can be exported as a polygon
void linesintopolygons(linelistlist in, planelist planes, int maxzone); //Final stage. Takes a list of lines sorted by planes, a list of planes and the max zone I care about, and then exports the resulting polygons

//----------------------------------------------------Source Headers-----------------------------------------------
void ui();

#endif