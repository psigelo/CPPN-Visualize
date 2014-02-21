
#include "cppn.hpp"

using namespace ANN_USM;

/**********************************************************************************************************************
	Connection
***********************************************************************************************************************/

void connection_gene::c_g(int innovation, int in, int out, double weight, bool enable)
{
	this->innovation = innovation;
	this->in = in;
	this->out = out;
	this->weight = weight;
	this->enable = enable;
	this->exist = true;
}

void connection_gene::c_g(bool exist)
{
	this->exist = exist;
}

int connection_gene::get_in()
{
	return this->in;
}

int connection_gene::get_out()
{
	return this->out;
}

int connection_gene::get_innovation()
{
	return this->innovation;
}

double connection_gene::get_weight()
{
	return this->weight;
}

bool connection_gene::is_enable()
{
	return this->enable;
}

bool connection_gene::do_exist()
{
	return this->exist;
}

/**********************************************************************************************************************
	Node
***********************************************************************************************************************/

node_gene::node_gene()
{
	this->incoming_connections = 0;
	this->accumulative_result = 0;
	this->counter = 0;
	this->final_result = 0;
}

void node_gene::n_g(int node, string function, gene_type type)
{
	this->node = node;
	this->type = type;
	this->exist = true;
	this->function = new Function(function);
}

void node_gene::n_g(bool exist)
{
	this->exist = exist;
}

void node_gene::increase_incoming_connection()
{
	this->incoming_connections++;
}

void node_gene::reset_counter()
{
	this->counter = 0;
}

int node_gene::get_node()
{
	return this->node;
}

/*! \brief 	Evaluate the incoming value from another node into the current node.
 *
 *			While there are incoming connections left, the evaluation consist only in the sum of the accumulative
 *	result with the new value. Now if the current evaluation is the last one in the node *(there are no more incoming 
 *	connections left)*, the accumulative result is evaluated in the node's function and assigned to the 'final_result'
 *	variable.
*/
void node_gene::eval(double value)
{
	this->accumulative_result += value;
	this->counter++;
	
	if(this->is_ready())
	{
		this->final_result = this->function->eval(this->accumulative_result);
		this->accumulative_result = 0;
	}
}

bool node_gene::is_ready()
{
	return this->incoming_connections == this->counter;
}

bool node_gene::do_exist()
{
	return this->exist;
}

double node_gene::get_final_result()
{
	return this->final_result;
}

gene_type node_gene::get_type()
{
	return this->type;
}

//**********************************************************************************************************************
//	CPPN
//**********************************************************************************************************************

void Genome::add_connection(int innovation, int in, int out, double weight)
{
	int list_size(Lconnection_genes.size());

	if(innovation >= list_size)
	{
		connection_gene missing_connection_gene;
		connection_gene new_connection;

		missing_connection_gene.c_g(false); // connection that not exist in this genome
		new_connection.c_g(innovation,in,out,weight,true);

		for (int i = 0; i < innovation-list_size; ++i)
		{
			Lconnection_genes.push_back(missing_connection_gene);
		}
		Lconnection_genes.push_back(new_connection);
	}
	else 
	{
		if(Lconnection_genes[innovation].do_exist())
		{
			cerr << "ERROR::In function add_connection, you wanted to add a connection gene with an innovation that already exists" << endl;	
			return;
		}
		else
			Lconnection_genes[innovation].c_g(innovation,in,out,weight,true);
	}

	this->Lnode_genes.at(out).increase_incoming_connection();
}

void Genome::add_node(int node, string function, gene_type type)
{
	int list_size(Lnode_genes.size());

	if(node >= list_size)
	{
		node_gene missing_node_gene;
		node_gene new_node;

		missing_node_gene.n_g(false);
		new_node.n_g(node, function, type);

		for (int i = 0; i < node - list_size; ++i)
			Lnode_genes.push_back(missing_node_gene);

		if(type == INPUT) new_node.increase_incoming_connection();

		this->Lnode_genes.push_back(new_node);
	}
	else
		if(this->Lnode_genes[node].do_exist())
		{
			cerr << "ERROR::In function add_node , you wanted to add a node gene with a node number that already exists" << endl;	
			return;
		}
		else
			this->Lnode_genes[node].n_g(node, function, type);

	if(type == INPUT) this->input_nodes.push_back(node);
	else if(type == OUTPUT) this->output_nodes.push_back(node);
}

/*! \brief 	Returns all the connections that have the specific node id as an outgoing one.
*/
vector<connection_gene> Genome::get_outgoing_connections(int node)
{
	vector<connection_gene> outgoing_connections;

	for (int i = 0; i < (int)this->Lconnection_genes.size(); i++)
	{
		if(	this->Lconnection_genes.at(i).do_exist() && \
			this->Lconnection_genes.at(i).is_enable() && \
			this->Lconnection_genes.at(i).get_in() == node)
			outgoing_connections.push_back(this->Lconnection_genes.at(i));
	}
	
	return outgoing_connections;
}

/*! \brief 	Recursive function that spreads the final result of the current node to all the connected nodes and so on.
*/
void Genome::spread_final_result(int node, double value)
{
	// Evaluates the node
	this->Lnode_genes.at(node).eval(value);

	if(this->Lnode_genes.at(node).is_ready())
	{
		this->Lnode_genes.at(node).reset_counter();

		value = this->Lnode_genes.at(node).get_final_result();

		vector<connection_gene> outgoing_connections = this->get_outgoing_connections(node);

		for (int i = 0; i < (int)outgoing_connections.size(); i++)
		{
			//cout << "     --> i: " << i << " spread to node " << outgoing_connections.at(i).get_out() << endl;
			this->spread_final_result(outgoing_connections.at(i).get_out(), value * outgoing_connections.at(i).get_weight());
		}
	}
}

/*! \brief 	Evaluation function used to get the output vector from the input vector.
*			This function represents the purpose of the CPPN.
*/
vector<double> Genome::eval(vector<double> inputs)
{
	vector<double> outputs;

	if(inputs.size() != this->input_nodes.size())
	{
		cerr << "error in function 'Genome::eval'. Number of input values differ from number of input nodes." << endl;
	}
	else
	{
		// Spread the initial values through the network
		for (int i = 0; i < (int)this->input_nodes.size(); i++)
			this->spread_final_result(this->input_nodes.at(i), inputs.at(i));

		// Recollect all the final results from the output nodes
		for (int i = 0; i < (int)this->output_nodes.size(); i++)
			outputs.push_back(this->Lnode_genes.at(this->output_nodes.at(i)).get_final_result());	
	}

	return outputs;
}

/*! \brief 	Overload of the operator '<<' with the purpose of printing JSON files.
*/
ostream & operator<<(ostream & o, Genome & encoding) 
{
	o << encoding.JSON();	
	return o;
}

string Genome::JSON()
{
	stringstream o;

	o << "{\n\t\"Genome\":\n\t{\n\t\t\"nodes\":\n\t\t[\n";

	int node_size(Lnode_genes.size());
	int connection_size(Lconnection_genes.size());

	for (int i = 0; i < node_size; ++i)
	{
		if(Lnode_genes[i].do_exist())
			o << "\t\t\t{\"exist\": " << Lnode_genes[i].do_exist()  << ",\"node\": " <<Lnode_genes[i].get_node() << ",\"type\": " << Lnode_genes[i].get_type();
		else
			o << "\t\t\t{\"exist\": " << Lnode_genes[i].do_exist() ;

		if(i<node_size-1)
			o <<  "},\n";
		else
			o <<  "}\n";
	}

	o << "\t\t],\n\t\t\"connection_genes\":\n\t\t[\n";
	for (int i = 0; i < connection_size; ++i)
	{
		if(Lconnection_genes[i].do_exist())
			o << "\t\t\t{\"exist\": " << Lconnection_genes[i].do_exist() << ",\"innovation\": " << Lconnection_genes[i].get_innovation() << ",\"in\": " << Lconnection_genes[i].get_in() << ",\"out\": " << Lconnection_genes[i].get_out() << ",\"weight\": " << Lconnection_genes[i].get_weight() << ",\"enable\": " << Lconnection_genes[i].is_enable();
		else
			o << "\t\t\t{\"exist\": " << Lconnection_genes[i].do_exist();
		
		if(i<connection_size-1)
			o <<  "},\n";
		else
			o <<  "}\n";
	}
	
	o << "\t\t]\n\t}\n}";
	return o.str();
}


void Genome::save(char path[])
{
	ofstream file;
	file.open (path);
	file << JSON();
	file.close();
}

void Genome::load(char path[])
{

	node_gene Nnew_node;
	connection_gene Cnew_node;

	Lconnection_genes.clear(); 
	Lnode_genes.clear();

	ifstream file (path);
	file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);
	char buffer[length]; // In JSON format
	file.read (buffer,length);
	file.close();

	bool exist;
	int node;
	int type;
	int innovation;
	int in;
	int out;
	double weight;
	int enable;
	int contador(0);
	bool connection(false);

	char * pch;
	char delimiters[] = " \n\":\t{},[";
	pch = strtok (buffer,delimiters);
	
	do{
		pch = strtok (NULL, delimiters);
		if(!(pch[0] == ']')){
			if(connection){
				if(!strncmp(pch, "exist",5)){
					pch = strtok (NULL, delimiters);
					exist = atoi(pch);
					if(exist){
						pch = strtok (NULL, delimiters);
						pch = strtok (NULL, delimiters);
						innovation = atoi(pch);
						pch = strtok (NULL, delimiters);
						pch = strtok (NULL, delimiters);
						in = atoi(pch);
						pch = strtok (NULL, delimiters);
						pch = strtok (NULL, delimiters);
						out = atoi(pch);
						pch = strtok (NULL, delimiters);
						pch = strtok (NULL, delimiters);
						weight = (double)atof(pch);
						pch = strtok (NULL, delimiters);
						pch = strtok (NULL, delimiters);
						enable = atoi(pch);
						Cnew_node.c_g(innovation, in, out, weight, (bool)enable);
						Lconnection_genes.push_back(Cnew_node);
					}
					else{
						Cnew_node.c_g(false);
						Lconnection_genes.push_back(Cnew_node);
					}
				} 
			}
			else{
				if(!strncmp(pch, "exist",5)){
					pch = strtok (NULL, delimiters);
					exist = atoi(pch);
					if(exist){
						pch = strtok (NULL, delimiters);
						pch = strtok (NULL, delimiters);
						node = atoi(pch);
						pch = strtok (NULL, delimiters);
						pch = strtok (NULL, delimiters);
						type = atoi(pch);
						Nnew_node.n_g(node, "IDENTITY", (gene_type) type);
						Lnode_genes.push_back(Nnew_node);
					}
					else{
						Nnew_node.n_g(false);
						Lnode_genes.push_back(Nnew_node);
					}
				} 
			}
		}
		else{
			contador=contador+1;
			connection = true;
			if(contador==2){
				break;
			}
		}
	}while (pch != NULL);
}