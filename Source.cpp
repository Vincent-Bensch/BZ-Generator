#include "Header.h"

void main() {
	init();
	vmath::configuration a("test.config");
	a.display();
	a.run();
}