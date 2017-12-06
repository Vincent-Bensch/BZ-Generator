#include "Header.h"

//---------------------------------------------------Variables--------------------------------------------------
string folder;
string logloc;
string outloc;
string configloc;
int dispspacing;
string space;
vmath::vector origin(0, 0, 0); //Vector at 0,0,0



//--------------------------------------------------Sub Routines------------------------------------------------
void init(){ //Initializing global variables
	folder = timestamp(true); //The global folder sting is equal to YYYY-MM-DD hhmm
	CreateDirectory(wstring(folder.begin(), folder.end()).c_str(), NULL); //Create a directory named the contents of the folder variable
	logloc = folder + "/Log.txt"; //Put the log file in our folder
	configloc = "test.config";
	dispspacing = 35;
	space = " ";
}

void togglescreen(){ ::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000); } //Toggle full screen

string spaceout(stringlist in) {
	string out;
	for (stringlist::size_type i = 0; i != in.size(); i++) { 
		out += in[i];
		if (i != in.size() - 1) {
			for (int j = 0; j != dispspacing - in[i].size(); j++) {
				out += " ";
			}
		}
	}
	return out;
}

string twodigit(int in) { //Takes a one or two digit integer and returns a sting of the int. If the int only has one digit, a 0 is added to compensate
	string out = str(in); //Make a sting of the input integer
	if (out.length() == 1) { out = "0" + out; } //If the string is one character long, add a 0 before it
	return out; //Return string
}

string timestamp(bool file) { //Create a timestamp either for a filename or log content
	time_t now = time(0); //Create placeholder at current time
	tm *ltm = localtime(&now); //Point to it
	string YYYY = str(1900 + ltm->tm_year); //Create stings of all relevant components for timestamp
	string MM = twodigit(1 + ltm->tm_mon);
	string DD = twodigit(ltm->tm_mday);
	string hh = twodigit(ltm->tm_hour);
	string mm = twodigit(ltm->tm_min);
	string ss = twodigit(ltm->tm_sec);

	if (file) { return YYYY + "-" + MM + "-" + DD + " " + hh + mm; } //Returns YYYY-MM-DD hhmm if timestamp is meant for a filename 
	else { return YYYY + "-" + MM + "-" + DD + " " + hh + ":" + mm + ":" + ss; } //Returns YYYY-MM-DD hh:mm:ss if not
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