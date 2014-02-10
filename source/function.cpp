
#include "function.hpp"

Function::Function(function_t function) : Pi(3.1415927), E(exp(1))
{
	switch(function)
	{
		case SIN: 
							this->function = &Function::Sin;		
							function_name = "Sine";				
		break;
		case COS:		
							this->function = &Function::Cos;		
							function_name = "Cosine";			
		break;
		case IDENTITY:	
							this->function = &Function::Identity;	
							function_name = "Identity";			
		break;
		case GAUSSIAN:	
							this->function = &Function::Gaussian;	
							function_name = "Gaussian";

							// Normal Gaussian Parameters
							param.push_back(1/sqrt(2 * Pi));
							param.push_back(0.0);
							param.push_back(1.0);
							param.push_back(0.0);
		break;
		case ABS:		
							this->function = &Function::Abs;		
							function_name = "Absolute value";	
		break;
	}
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