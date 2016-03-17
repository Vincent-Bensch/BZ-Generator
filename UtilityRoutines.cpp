#include "Header.h"

//---------------------------------------------------Variables--------------------------------------------------
string logloc;
string outloc;
vmath::vector origin(0, 0, 0); //Vector at 0,0,0

//--------------------------------------------------Constructors------------------------------------------------
vmath::vector::vector(double a, double b, double c) { i = a, j = b, k = c; } //Constructor for populating vector
vmath::vector::vector() {} //Default constructor for vector

vmath::plane::plane(vector a, vector b) { norm = a, loc = b, r = norm.dot(loc); } //Constructor for populating plane and calculating r
vmath::plane::plane() {} //Default constructor for plane

vmath::line::line(vector a, vector b) { dir = a, loc = b; } //Constructor for populating line
vmath::line::line() {} //Default constructor for line

vmath::linesegment::linesegment(vector a, vector b) { start = a, end = b; } //Function header for line segment populating constructor
vmath::linesegment::linesegment() {} //Function header for default constructor

//--------------------------------------------------Sub Routines------------------------------------------------
void init(){ //Initializing global variables
	logloc = "Log.txt";
}

string timestamp() { //Returns YYYY-MM-DD hh:mm:ss
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return str(1900 + ltm->tm_year) + "-" + str(1 + ltm->tm_mon) + "-" + str(ltm->tm_mday) + " " + str(1 + ltm->tm_hour) + ":" + str(1 + ltm->tm_min) + ":" + str(1 + ltm->tm_sec);
}

void report(string in, bool file, bool screen) { //Timestamps message and then posts it to screen and writes it to log file
	string out = timestamp() + "	" + in; //Timestamp message

	if (file) { //If write to file is requested
		filebuf outfile; //Create file buffer
		outfile.open(logloc, ios_base::app | ios_base::out); //Open file buffer
		ostream outstream(&outfile); //Assign file buffer to stream
		outstream << out << "\n"; //Write message to stream
		outfile.close(); //Close file buffer
	}

	if (screen) { //If screen print is requested
		cout << out << endl; //Write message to screen
	}
}

void writepolygon(vectorlist in, int zone) { //Write a polygon to a .obj file
	filebuf outfile; //Create file buffer
	outfile.open(outloc + " " + str(zone) + ".obj", ios_base::app | ios_base::out); //Open file buffer
	ostream outstream(&outfile); //Assign file buffer to stream

	outstream << "f"; //Line header for polygon
	for (vectorlist::size_type i = 0; i != in.size(); i++) { outstream << " " << in[i].i << "/" << in[i].j << "/" << in[i].k; } //Add all points in the input list to the file
	outstream << "\n"; //End line

	outfile.close(); //Close file buffer
}

int identifyzone(vmath::vector in, planelist planes) { //Identify Brillouin zone a point belongs to
	int out = 0; //Starting in zone 0, because all my points are going to be on the edge of the zone, artificially inflating the zone count
	vmath::linesegment segment(origin, in); //Create line segment from the origin to the point being tested
	for (planelist::size_type i = 0; i != planes.size(); i++) { if (segment.intersect(planes[i])) { out++; } } //While iterating over planes, if current plane intersects segment, increment zone by 1
	return out; //Return zone number
}

bool isorigin(vmath::vector in) { return origin.equals(in); } //Is inputted vector origin?

vectorlist removeorigin(vectorlist in) { //Remove instances origin from list of vectors
	in.erase(remove_if(in.begin(), in.end(), isorigin), in.end()); //Remove all elements that cause isorigin to return true
	return in; //Return input list
}

string requeststring(string ask) { //Prints ask string and then returns user input as a string
	string out; //Create input string
	cout << ask << ": "; //Print ask string
	cin >> out; //Take input
	return out; //Return input
}

int requestint(string ask) { //Prints ask string and then returns user input as a int
	int out; //Create input int
	cout << ask << ": "; //Print ask string
	cin >> out; //Take input
	return out; //Return input
}

double requestdouble(string ask) { //Prints ask string and then returns user input as a double
	double out; //Create input double
	cout << ask << ": "; //Print ask string
	cin >> out; //Take input
	return out; //Return input
}