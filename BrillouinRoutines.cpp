#include "Header.h"

//--------------------------------------------------Sub Routines------------------------------------------------
vectorlist makePClattice(double max, double step) { //Creates a Primitive Cube lattice
	vectorlist cash; //Creates output list

	for (double x = -1 * max; x <= max; x = x + step) { //Passing from -max to max using step
		for (double y = -1 * max; y <= max; y = y + step) {
			for (double z = -1 * max; z <= max; z = z + step) {
				cash.push_back(vmath::vector( //Adds current point to output cash
					x,
					y,
					z));
			}
		}
	}
	return removeorigin(cash); //Return output list - origin
}

vectorlist makeBCClattice(double max, double step) { //Creates a Body Centered Cubic lattice
	vectorlist cash;
	double halfstep = step / 2;

	for (double x = -1 * max; x <= max; x = x + step) { //Passing from -max to max using step
		for (double y = -1 * max; y <= max; y = y + step) {
			for (double z = -1 * max; z <= max; z = z + step) {
				cash.push_back(vmath::vector( //Adds current point to output cash. The point is then modified to get the other points that make up a BCC lattice
					x,
					y,
					z));

				cash.push_back(vmath::vector(
					x + halfstep,
					y + halfstep,
					z));

				cash.push_back(vmath::vector(
					x + halfstep,
					y,
					z + halfstep));

				cash.push_back(vmath::vector(
					x,
					y + halfstep,
					z + halfstep));
			}
		}
	}
	return removeorigin(cash); //Return output list - origin
}

vectorlist makeFCClattice(double max, double step) { //Creates a Face Centered Cubic lattice
	vectorlist cash;
	double halfstep = step / 2;

	for (double x = -1 * max; x <= max; x = x + step) { //Passing from -max to max using step
		for (double y = -1 * max; y <= max; y = y + step) {
			for (double z = -1 * max; z <= max; z = z + step) { 
				cash.push_back(vmath::vector( //Adds current point to output cash. The point is then modified to get the other points that make up a FCC lattice
					x,
					y,
					z));

				cash.push_back(vmath::vector(
					x + halfstep,
					y + halfstep,
					z + halfstep));
			}
		}
	}
	return removeorigin(cash); //Return output list - origin
}

linesegmentlist makelinesegmentsfromlattice(vectorlist in) {
	linesegmentlist out;
	for (vectorlist::size_type i = 0; i != in.size(); i++) { out.push_back(vmath::linesegment(origin, in[i])); }
	return out;
}

planelist makebisectorplanes(linesegmentlist in) { //Takes a list of line segments, and outputs a list of planes bisecting those line segments
	planelist out; //Create output list
	for (linesegmentlist::size_type i = 0; i != in.size(); i++) { out.push_back(vmath::plane(in[i].end.subtract(in[i].start), in[i].midpoint())); } //For all line segments in input list, make plane using start-end as normal and midpoint as point
	return out; //Return output list
}

vectorlist makepolygon(linesegmentlist in) { //Takes a list of filtered but unsorted line-segments and organizes them into a sorted vectorlist that can be exported as a polygon
	vectorlist out; //Create vectorlist for output
	out.push_back(in[0].start); //Assign the start of the first line segment to the beginning of the output list
	vmath::vector endpoint = in[0].start; //Hold the end of the first line segment in a buffer
	in.erase(in.begin()); //Remove the first line segment from the input list

	while (in.size() != 0) { //While there are still line segments in the input list
		for (linesegmentlist::size_type i = 0; i != in.size(); i++) { //Iterate through the list of line segments
			if (in[i].start.equals(out[out.size() - 1])) { //If the start point of the current line segment is equal to the last point of the output list
				out.push_back(in[i].end); //Add the endpoint of the current line segment to the output list
				in.erase(in.begin() + i); //Delete current line segment from input list
				break; //Restart the for loop
			}

			if (in[i].end.equals(out[out.size() - 1])) { //If the start point of the current line segment is equal to the last point of the output list
				out.push_back(in[i].start); //Add the endpoint of the current line segment to the output list
				in.erase(in.begin() + i); //Delete current line segment from input list
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