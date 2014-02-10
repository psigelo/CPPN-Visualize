
#include "function.hpp"

Function::Function(function_t function)
{
	switch(function)
	{
		case SIN:		this->function = &Function::Sin;		function_name = "Sine";				break;
		case COS:		this->function = &Function::Cos;		function_name = "Cosine";			break;
		case IDENTITY:	this->function = &Function::Identity;	function_name = "Identity";			break;
		case GAUSSIAN:	this->function = &Function::Gaussian;	function_name = "Gaussian";			break;
		case ABS:		this->function = &Function::Abs;		function_name = "Absolute value";	break;
	}

	a = 1/sqrt(2 * Pi);
	b = 0.0;
	c = 1.0;
	d = 0.0;
}

float Function::eval(float input)
{
	return (this->*function)(input);
}

string Function::get_name()
{
	return function_name;
}

// Functions
float Function::Sin(float input)
{
	return sin(input);
}

float Function::Cos(float input)
{
	return cos(input);
}

float Function::Identity(float input)
{
	return input;
}

float Function::Gaussian(float input)
{
	return a * exp(pow(input-b,2)/(2*pow(c,2))) + d;
}

float Function::Abs(float input)
{
	return abs(input);
}