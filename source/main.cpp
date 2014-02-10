
#include "cppn.hpp"

using namespace ANN_USM;

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		cout << "Usage: " << argv[0] << " <input_file_name> <output_file_name>" << endl;
		return -1;
	}

	CPPN * cppn = new CPPN();

	// Set the output file if defined
	if(argc > 2) cppn->set_file_name(argv[2]);

	ifstream file;
	file.open (argv[1]);

	string specifier;
	while (file.good()) {

		file >> specifier;

		if(specifier == "RESOLUTION")
		{
			int x_res;
			int y_res;

			file >> x_res;
			file >> y_res;

			cppn->set_resolution(x_res, y_res);
		}
		else if(specifier == "CONSTRAINT")
		{
			float x_max;
			float x_min;
			float y_max;
			float y_min;

			file >> x_max;
			file >> x_min;
			file >> y_max;
			file >> y_min;

			cppn->set_cartesian_constraints(x_max, x_min, y_max, y_min);
		}
		else if(specifier == "NODE")
		{
			int node_num;
			string function_name;
    		
			file >> node_num;
			for (int i = 0; i < node_num; i++)
			{
				file >> function_name;

				cppn->add_node(function_name);
			}
		}
		else if(specifier == "CONNECTION")
		{
			int connection_num;
			int node_1;
			int node_2;
			float weight;

			file >> connection_num;
			for (int i = 0; i < connection_num; i++)
			{
				file >> node_1;
				file >> node_2;
				file >> weight;

				cppn->add_connection(node_1, node_2, weight);
			}
		}
		else if(specifier == "INPUT")
		{
			int node_1;
			int node_2;

			file >> node_1;
			file >> node_2;

			cppn->set_input(node_1, node_2);
		}
		else if(specifier == "OUTPUT")
		{
			int node;

			file >> node;

			cppn->set_output(node);
		}
	}

	file.close();

	// Evaluates the plane of points
	cppn->eval();

	// Print the CPPN
	// ONLY FOR DEBUGGING PURSPOSE
	cppn->print();

	return 0;	
}