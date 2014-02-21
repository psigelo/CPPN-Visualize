
#ifndef VISUALATOR_HH
#define VISUALATOR_HH

#include <fstream>
#include <vector>
#include <string>
//#include <cstdlib>

#include "cppn.hpp"

using namespace std;

namespace ANN_USM
{
	class Input
	{
		public:

			Input(string type, vector<double> params);

			double eval(double x, double y);

		private:

			vector<double> params;

			double (Input::*function)(double x, double y);

			// Cartesian
			double cartesian(double x, double y);

			// Bias
			double bias(double x, double y);

			// Radius
			double radius(double x, double y);

			// USER_CUSTOM
			//double user_custom(double x, double y);
	};

	class Visualator
	{
		public:

			Visualator();

			void load_file(string file);
			void generate_json(string file);
			void set_resolution(int x_res, int y_res);
			void set_constraints(int x_max, int x_min, int y_max, int y_min);

			string dtoa(double value);

		private:

			Genome * genome;

			int x_res;
			int y_res;

			double x_max;
			double x_min;

			double y_max;	
			double y_min;

			vector<Input> inputs;
	};
}

#endif