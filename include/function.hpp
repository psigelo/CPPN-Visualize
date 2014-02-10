
#ifndef FUNCTION_HH
#define FUNCTION_HH

#include <cmath>
#include <string>
#include <vector>
#include <map>

using namespace std;

/*enum function_t {SIN, COS, IDENTITY, GAUSSIAN, ABS};

map<string, function_t> enumResolver;

enumResolver["SIN"] 		= SIN;
enumResolver["COS"] 		= COS;
enumResolver["IDENTITY"]	= IDENTITY;
enumResolver["GAUSSIAN"]	= GAUSSIAN;
enumResolver["ABS"]			= ABS;*/

class Function
{
	public:

		Function(string);
		float eval(float);
		string get_name();

	private:

		string function_name;

		// Useful numbers
		const float Pi;
		const float E;

		// Function Parameters, if any
		vector<float> param;

		// Function pointer
		float (Function::*function)(float);

		// Functions
		float Sin(float);
		float Cos(float);
		float Identity(float);
		float Gaussian(float);
		float Abs(float);
};

#endif