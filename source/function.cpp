
#include "function.hpp"

Function::Function(string function_name) : Pi(M_PI), E(exp(1))
{
	if(function_name == "SIN") 				this->function = &Function::Sin;	
	else if(function_name == "COS")			this->function = &Function::Cos;
	else if(function_name == "GAUSSIAN")
	{	
		this->function = &Function::Gaussian;
		param.push_back(1/sqrt(2 * Pi));
		param.push_back(0.0);
		param.push_back(1.0);
		param.push_back(0.0);
	}
	else if(function_name == "ABS")			this->function = &Function::Abs;
	else if(function_name == "IDENTITY")	this->function = &Function::Identity;

	this->function_name = function_name;
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
	return param.at(0) * exp(pow(input-param.at(1),2)/(2*pow(param.at(2),2))) + param.at(3);
}

float Function::Abs(float input)
{
	return abs(input);
}