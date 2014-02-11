
#include "cppn.hpp"
#include <cstring>

using namespace ANN_USM;

void print_usage(string proc)
{
	cout << "Usage:" << endl << "\t" << proc << " -i input_file_name [-o output_file_name] [-g]" << endl << endl;
	/* TO DO: Improve this section*/
}

int main(int argc, char *argv[])
{
	string input_file("");
	string output_file("");

	bool g_flag = false;

	for (int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-g"))
		{
			g_flag = true;
		}
		else if(!strcmp(argv[i], "-i"))
		{
			if(++i < argc) input_file = argv[i];
			else
			{
				cerr << "error: missing input file name after -i" << endl;
				print_usage(argv[0]);
				return -1;
			}
		}
		else if(!strcmp(argv[i], "-o"))
		{
			if(++i < argc) output_file = argv[i];
			else
			{
				cerr << "error: missing output file name after -o" << endl;
				print_usage(argv[0]);
				return -1;
			}
		}
		else if(!strcmp(argv[i], "-h"))
		{
			print_usage(argv[0]);
			return 0;
		}
	}

	// Check input file
	if(input_file == "")
	{
		cerr << "error: missing input file." << endl;
		print_usage(argv[0]);
		return -1;
	}

	// Check output file
	if(output_file == "")
	{
		output_file = input_file + ".out";
	}

	/******************
		Start CPPN
	*******************/

	CPPN * cppn = new CPPN(g_flag, output_file);

	ifstream file(input_file);
	
	if(file.is_open())
	{
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
	}
	else
	{
		cerr << "error: make sure the file '" << input_file << "' exists." << endl;
		return -1;
	}

	file.close();

	// Evaluates the plane of points
	cppn->eval();

	// Print the CPPN
	// ONLY FOR DEBUGGING PURSPOSE
	//cppn->print();

	return 0;	
}