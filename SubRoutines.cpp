#include "Header.h"

//---------------------------------------------------Variables--------------------------------------------------
double intdist;
string logloc;

//--------------------------------------------------Constructors------------------------------------------------
vmath::vector::vector(double a, double b, double c) { i = a, j = b, k = c; } //Constructor for populating vector
vmath::vector::vector() {} //Default constructor for vector

vmath::plane::plane(vector a, vector b) { norm = a, loc = b, r = norm.dot(loc); } //Constructor for populating plane and calculating r
vmath::plane::plane() {} //Default constructor for plane

vmath::line::line(vector a, vector b) { dir = a, loc = b; } //Constructor for populating line
vmath::line::line() {} //Default constructor for line

vmath::linesegment::linesegment(vector a, vector b) { start = a, end = b; } //Function header for linesegment populating constructor
vmath::linesegment::linesegment() {} //Function header for default constructor

//--------------------------------------------------Sub Routines------------------------------------------------
void init(){ //Initializing global variables
	intdist = 0.0001;
}

string timestamp() { //Returns YYYY-MM-DD hh:mm:ss
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return str(1900 + ltm->tm_year) + "-" + str(1 + ltm->tm_mon) + "-" + str(ltm->tm_mday) + " " + str(1 + ltm->tm_hour) + ":" + str(1 + ltm->tm_min) + ":" + str(1 + ltm->tm_sec);
}

void report(string in, bool file = true, bool screen = true) { //Timestapms message and then posts it to screen and writes it to log file
	string out = timestamp() + "	" + in; //Timestamp message

	if (file) { //If write to file is requested
		filebuf outfile; //Create file buffer
		outfile.open(logloc, ios::out); //Open file buffer
		ostream outstream(&outfile); //Assign file buffer to stream
		outstream << out << "\n"; //Write message to stream
		outfile.close(); //Close file buffer
	}

	if (screen) { //If screen print is requested
		cout << out << endl; //Write message to screen
	}
}

planelist makebisectorplanes(linesegmentlist in) { //Takes a list of linesegments, and outputs a list of planes bisecting those linesegments
	planelist out; //Create output list
	for (linesegmentlist::size_type i = 0; i != in.size(); i++) { out.push_back(vmath::plane(in[i].end.subtract(in[i].start), in[i].midpoint())); } //For all line segments in input list, make plane using start-end as normal and midpoint as point
	return out; //Return output list
}

vectorlist makepolygon(linesegmentlist in) { //Takes a list of filtered but unsorted line-segments and organizes them into a sorted vectorlist that can be exported as a polygon
	vectorlist out; //Create vectorlist for output
	out.push_back(in[0].start); //Asign the start of the first linesegment to the begining of the output list
	vmath::vector endpoint = in[0].start; //Hold the end of the first linesegment in a buffer
	in.erase(in.begin()); //Remove the first linesegment from the input list

	while (in.size() != 0) { //While there are still linesegments in the input list
		for (linesegmentlist::size_type i = 0; i != in.size(); i++) { //Iterate through the list of linesegments
			if (in[i].start.equals(out[out.size() - 1])) { //If the start point of the current linesegment is equal to the last point of the output list
				out.push_back(in[i].end); //Add the endpoint of the cureent linesegment to the output list
				in.erase(in.begin() + i); //Delete current linesegment from input list
				break; //Restart the for loop
			}

			if (in[i].end.equals(out[out.size() - 1])) { //If the start point of the current linesegment is equal to the last point of the output list
				out.push_back(in[i].start); //Add the endpoint of the cureent linesegment to the output list
				in.erase(in.begin() + i); //Delete current linesegment from input list
				break; //Restart the for loop
			}
		}
	}

	if (endpoint.equals(out[out.size() - 1])) { //The buffered endpoint should be equal to the last point in the output list
		vectorlist e; //If not create an empty list
		return e; //And then return it
	}

	return out; //Return output list
}