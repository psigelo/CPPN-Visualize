
#include "visualator.hpp"

using namespace ANN_USM;

/*******************************************************************************************
	Input
*******************************************************************************************/

Input::Input(string type, vector<double> params)
{
	this->params = params;

	if(type == "CARTESIAN")
	{
		function = &Input::cartesian;
	}
	else if(type == "BIAS")
	{
		function = &Input::bias;
	}
	else if(type == "RADIUS")
	{
		function = &Input::radius;
	}
	//else if(type == "USER_CUSTOM")
	//{
	//	function = &Input::user_custom;
	//}
}

double Input::eval(double x, double y)
{
	return (*this.*function)(x, y);
}

double Input::bias(double x, double y)
{
	return this->params.at(0);
}

double Input::radius(double x, double y)
{
	return sqrt(pow(x, 2) + pow(y, 2));
}

double Input::cartesian(double x, double y)
{
	return (this->params.at(0)) ? y : x;
}

/*******************************************************************************************
	Visualator
*******************************************************************************************/

Visualator::Visualator()
{
	this->genome = new Genome();
}

void Visualator::load_file(string file_name)
{
	ifstream file(file_name.c_str());
	
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

				this->set_resolution(x_res, y_res);
			}
			else if(specifier == "CONSTRAINTS")
			{
				double x_max;
				double x_min;
				double y_max;
				double y_min;

				file >> x_max;
				file >> x_min;
				file >> y_max;
				file >> y_min;

				this->set_constraints(x_max, x_min, y_max, y_min);
			}
			else if(specifier == "NODES")
			{
				int node_num;

				string type;
				string function;

				file >> node_num;

				for (int i = 0; i < node_num; i++)
				{
					file >> type;
					file >> function;

					gene_type g_type;

					if(type == "INPUT") g_type = INPUT;
					else if(type == "OUTPUT") g_type = OUTPUT;
					else if(type == "HIDDEN") g_type = HIDDEN;

					this->genome->add_node(i, function, g_type);
				}
			}
			else if(specifier == "CONNECTIONS")
			{
				int connection_num;
				int node_1;
				int node_2;
				double weight;

				file >> connection_num;
				for (int i = 0; i < connection_num; i++)
				{
					file >> node_1;
					file >> node_2;
					file >> weight;

					this->genome->add_connection(i, node_1, node_2, weight);
				}
			}
			else if(specifier == "INPUTS")
			{
				vector<Input> inputs;
				vector<double> params;

				// Defining the cartesian 'x' & 'y'
				params.push_back(0);
				Input input_1("CARTESIAN", params);
				vector<double>().swap(params); // clear

				params.push_back(1);
				Input input_2("CARTESIAN", params);				
				vector<double>().swap(params); // clear

				inputs.push_back(input_1);
				inputs.push_back(input_2); 

				int input_num;

				file >> input_num;

				for (int i = 0; i < input_num; i++)
				{
					file >> specifier;

					if(specifier == "BIAS")
					{
						double bias;

						file >> bias;

						params.push_back(bias);
					}
					else if(specifier == "RADIUS")
					{
						// nothing special to do here
					}		

					Input input(specifier, params);	
					inputs.push_back(input);
					vector<double>().swap(params); // clear
				}

				this->inputs = inputs;
			}
		}

		file.close();
	}
	else cerr << "error: make sure the file '" << file << "' exists." << endl;	
}

string Visualator::dtoa(double value)
{
	ostringstream s;
	s << value;
	return s.str();
}

void Visualator::generate_json(string file_name)
{
	string json;

	vector<double> inputs;
	vector<double> outputs;

	json.append("{\"resolution\":{\"x\":");
	json.append(this->dtoa(this->x_res));
	json.append(",\"y\":");
	json.append(this->dtoa(this->y_res));
	json.append("},\"values\":[");

	double x_unit = abs(x_max - x_min) / x_res;
	double y_unit = abs(y_max - y_min) / y_res;

	double y = this->y_max;
	for (int i = 0; i < y_res; i++)
	{
		json.append("[");

		double x = this->x_min;
		for (int j = 0; j < x_res; j++)
		{
			for (int i = 0; i < (int)this->inputs.size(); i++)
				inputs.push_back(this->inputs.at(i).eval(x,y));

			outputs = this->genome->eval(inputs);

			json.append(dtoa(outputs.at(0)));
			json.append((j < x_res - 1) ? "," : "");

			vector<double>().swap(inputs); // clear
			vector<double>().swap(outputs); // clear

			x += x_unit;
		}
		json.append("]");
		json.append((i < y_res - 1) ? "," : "");

		y -= y_unit;
	}
	json.append("]}");

	ofstream file(file_name.c_str());

	file << json;

	file.close();
}

void Visualator::set_resolution(int x_res, int y_res)
{
	this->x_res = x_res;
	this->y_res = y_res;
}

void Visualator::set_constraints(int x_max, int x_min, int y_max, int y_min)
{
	this->x_max = x_max;
	this->x_min = x_min;
	this->y_max = y_max;
	this->y_min = y_min;
}
