#include "Header.h"

void main() {
	init();
	ui();
}

void ui() {
	report("Brillouin Zone Generator User Interface Started");

	vectorlist uilatticepoints;
	linesegmentlist uilinesegmentlist;
	planelist uiplanelist;
	linelistlist uilinesbyplanes;

	string latticetype = requeststring("Lattice Type");
	double maxlatticedistance = requestdouble("Maximum distance from origin for Lattice Points");
	double latticestep = requestdouble("Step for Lattice Points");
	int maximumzone = requestint("Highest zone you care about");
	outloc = requeststring("Base filename for file output");
	cout << endl << endl;
	report("Parameters received \n			Lattice Type: " + latticetype 
		+ "\n			Maximum distance from origin for Lattice Points: " + str(maxlatticedistance) 
		+ "\n			Step for Lattice Points: " + str(latticestep)
		+ "\n			Highest zone you car about: " + str(maximumzone)
		+ "\n			Base filename for file output: " + outloc);

	if (latticetype == "PC") { uilatticepoints = makePClattice(maxlatticedistance, latticestep); }
	if (latticetype == "FCC") { uilatticepoints = makeFCClattice(maxlatticedistance, latticestep); }
	if (latticetype == "BCC") { uilatticepoints = makeBCClattice(maxlatticedistance, latticestep); }
	report("Lattice points generated");

	uilinesegmentlist = makelinesegmentsfromlattice(uilatticepoints);
	report("Line segments generated from lattice");

	uiplanelist = makebisectorplanes(uilinesegmentlist);
	report("Bisector planes generated");

	uilinesbyplanes = makelinesfromplanes(uiplanelist);
	report("Lines generated on planes");

	linesintopolygons(uilinesbyplanes, uiplanelist, maximumzone);
	report("Done");

	cin.ignore();
}