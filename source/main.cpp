
#include "visualator.hpp"
#include <cstring>

using namespace ANN_USM;

void print_usage(string proc)
{
	cout << "Usage:" << endl << "\t" << proc << " -i input_file_name [-o output_file_name]" << endl << endl;
	/* TO DO: Improve this section*/
}

int main(int argc, char *argv[])
{
	string input_file("");
	string output_file("");

	for (int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-i"))
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
		output_file = input_file + ".json";
	}

	Visualator * visualator = new Visualator();

	visualator->load_file(input_file);

	visualator->generate_json(output_file);

	return 0;	
}