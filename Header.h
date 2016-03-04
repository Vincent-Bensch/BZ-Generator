#pragma once
#ifndef HEADER_H_
#define HEADER_H_

//Includes and Defines
#include <iostream>
#include <sstream> //For str( x )
#include <ctime> //For Timer

#define str( x ) dynamic_cast< std::ostringstream & >( \
	(std::ostringstream() << std::dec << x)).str()

using namespace std;

//Classes
namespace vmath {

	class timer {
	public:
		double a;
		double b;
		void start() { a = clock(); return; }
		void stop() { b = (clock() - a) / (double)CLOCKS_PER_SEC; return; }
		double read() { return b; }
	};

	class vector {
	public:
		double i, j, k;

		void populate(double a, double b, double c) { i = a, j = b, k = c; return; }

		double magnitude() { return sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2)); }

		vector normalize() {
			vector o;
			double l = magnitude();
			o.populate(
				i / l,
				j / l,
				k / l);
			return o;
		}

		double dot(vector v) { return i*v.i + j*v.j + k*v.k; }

		vector cross(vector v) {
			vector o;
			o.populate(
				j*v.k - k*v.j,
				k*v.i - i*v.k,
				i*v.j - j*v.i);
			return o;
		}

		vector add(vector v) {
			vector o;
			o.populate(
				i + v.i,
				j + v.j,
				k + v.k);
			return o;
		}

		vector subtract(vector v) {
			vector o;
			o.populate(
				i - v.i,
				j - v.j,
				k - v.k);
			return o;
		}

		vector multiply(double d) {
			vector o;
			o.populate(
				i*d,
				j*d,
				k*d);
			return o;
		}

		vector divide(double d) {
			vector o;
			o.populate(
				i / d,
				j / d,
				k / d);
			return o;
		}
	};

	class line {
	public:
		vector direction;
		vector location;

		bool intersect(line i) {
			vector lcl = direction.cross(i.direction);
			return abs(i.location.subtract(direction).dot(lcl) / lcl.magnitude()) <= intdist;
		}

		vector intersection(line v) {
			return location.add(direction.multiply((direction.cross(v.direction).i) / (v.location.subtract(location).cross(v.direction).i)));
		}
	};

	class plane {
	public:
		vector location;
		vector normal;

		bool intersect(plane i) { return normal.dot(i.normal) == 1; }

		line intersection(plane i) {
			line o;
			normal = normal.normalize();
			i.normal = i.normal.normalize();
			o.direction = normal.cross(i.normal);

			double h1 = normal.dot(location);
			double h2 = i.normal.dot(i.location);
			double p1 = normal.dot(i.normal);
			double p2 = 1 - pow(p1, 2);

			o.location = normal.multiply((h1 - h2*p1) / p2).add(i.normal.multiply((h2 - h1*p1) / p2));
			return o;
		}
	};

	class linesegment {
	public:
		vector start, end;

		vector midpoint() {
			vector o;
			o.populate(
				(start.i + end.i) / 2,
				(start.j + end.j) / 2,
				(start.k + end.k) / 2);
			return o;
		}

		double length() { return sqrt(pow(end.i - start.i, 2) + pow(end.j - start.j, 2) + pow(end.k - start.k, 2)); }

		bool intersect(line i) {
			if (!has_dir) {
				ifline.direction = end.subtract(start);
				ifline.location = end;
				has_dir = true;
			}
			if (!ifline.intersect(i)) { return false; }
			intpoint = ifline.intersection(i);
			return ((start.i > intpoint.i > end.i) || (start.i <= intpoint.i <= end.i)) && ((start.j > intpoint.j > end.j) || (start.j <= intpoint.j <= end.j)) && ((start.k > intpoint.k > end.k) || (start.k <= intpoint.k <= end.k));
		}

		vector intersection(line i) { return intpoint; }

	private:
		line ifline;
		bool has_dir = false;
		vector intpoint;
	};
}

//External Variables
extern double intdist;

//Function Headers
void init();

#endif