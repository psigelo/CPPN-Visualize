
#include "cppn.hpp"

int main()
{
	// SIN, COS, IDENTITY, GAUSSIAN, ABS

	CPPN * cppn = new CPPN();

	// Nodes
	cppn->add_node(IDENTITY);	// 0
	cppn->add_node(IDENTITY);	// 1
	cppn->add_node(IDENTITY);	// 2
	cppn->add_node(IDENTITY);	// 3
	cppn->add_node(IDENTITY);	// 4
	cppn->add_node(IDENTITY);	// 5
	cppn->add_node(IDENTITY);	// 6

	// Connections
	cppn->add_connection(0,2,1.);
	cppn->add_connection(0,3,1.);
	cppn->add_connection(1,2,1.);
	cppn->add_connection(1,3,1.);
	cppn->add_connection(2,4,1.);
	cppn->add_connection(2,5,1.);
	cppn->add_connection(3,4,1.);
	cppn->add_connection(3,5,1.);
	cppn->add_connection(4,6,1.);
	cppn->add_connection(5,6,1.);

	// Define inputs
	cppn->set_input(0,1);

	// Define output
	cppn->set_output(6);

	// Set resolution 
	cppn->set_resolution(2,2);

	// Set cartesian constraints
	cppn->set_cartesian_constraints(1.0,-1.0,1.0,-1.0);

	// Evaluates the plane of points
	cppn->eval();

	// Print the CPPN
	cppn->print();

	return 0;	
}