
#ifndef FUNCTION_HH
#define FUNCTION_HH

#include <cmath>
#include <string>

using namespace std;

enum function_t {SIN, COS, IDENTITY, GAUSSIAN, ABS};

const float Pi = 3.1415927;

class Function
{
	public:

		Function(function_t);
		float eval(float);
		string get_name();

	private:

		string function_name;

		// Gaussian Default Parameters
		float a,b,c,d;

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