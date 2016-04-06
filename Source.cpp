#include "Header.h"

void main() {
	init();

	ui();

	cin.ignore();
}

void ui() {
		ifstream infile(configloc);
		configurationlist configlist;
		int a, d;
		double b, c;
		string e;

		togglescreen();
		while (infile >> a >> b >> c >> d >> e) { configlist.push_back(vmath::configuration(a, b, c, d, e)); }
		cout << spaceout({ "Option#", "Lattice Type", "Max Lattice Point Distance", "Lattice Step", "Maximum Zone" }) << endl;
		for (configurationlist::size_type i = 0; i != configlist.size(); i++) { cout << spaceout({ str(i), configlist[i].latticetype_s, str(configlist[i].maxlatticedistance), str(configlist[i].latticestep), str(configlist[i].maximumzone) }) << endl; }
		cout << spaceout({ str(configlist.size()), "Enter new configuration" }) << endl;
		int option = requestint("Option");
		togglescreen();
		if (option < configlist.size()) { configlist[option].run(); }
		else if (option == configlist.size()) { vmath::configuration().run();}
		else { report("Failstate in UI"); exit(0); }
}



