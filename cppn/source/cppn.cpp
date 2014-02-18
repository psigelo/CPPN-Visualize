
#include "cppn.hpp"

using namespace ANN_USM;

/**********************************************************************************************************************
	Connection
***********************************************************************************************************************/

void connection_gene::c_g(int innovation, int in, int out, double weight, bool enable)
{
	this->innovation=innovation;
	this->in=in;
	this->out=out;
	this->weight=weight;
	this->enable=enable;
	this->exist=true;
}

void connection_gene::c_g(bool exist)
{
	this->exist=exist;
}

void connection_gene::set_visited(bool value)
{
	this->visited = value;
}

int connection_gene::get_in()
{
	return this->in;
}

int connection_gene::get_out()
{
	return this->out;
}

bool connection_gene::is_enable()
{
	return this->enable;
}

bool connection_gene::do_exist()
{
	return this->exist;
}

bool connection_gene::is_visited()
{
	return this->visited;
}

/**********************************************************************************************************************
	Node
***********************************************************************************************************************/

node_gene::node_gene()
{
	this->incoming_connections = 0;
	this->accumulative_result = 0;
	this->final_result = 0;
}

void node_gene::n_g(int node, gene_type type)
{
	this->node = node;
	this->type = type;
	this->exist = true;
}

void node_gene::n_g(bool exist)
{
	this->exist = exist;
}

void node_gene::increase_incoming_connection()
{
	this->incoming_connections++;
}

void node_gene::set_visited(bool value)
{
	this->visited = value;
}

bool node_gene::is_ready()
{
	return this->incoming_connections == this->counter;
}

bool node_gene::do_exist()
{
	return this->exist;
}

bool node_gene::is_visited()
{
	return this->visited;
}

double node_gene::get_result()
{
	return this->final_result;
}

void node_gene::eval(double value)
{
	this->accumulative_result += value;
	this->counter++;

	if(this->is_ready())
	{
		this->result = this->function->eval(this->result);

		/*
		Node * aux_node;

		this->nav_connection = this->head_connection;

		for (int i = 0; i < this->num_connections; i++)
		{
			aux_node = this->nav_connection->get_connected_node();
			aux_node->eval(this->result * this->nav_connection->get_weight());
			this->nav_connection = this->nav_connection->get_next_connection();
		}
		*/

		this->accumulative_result = 0;
		this->counter = 0;
	}
}

/**********************************************************************************************************************
	CPPN
***********************************************************************************************************************/

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
		if(Lconnection_genes[innovation].exist)
		{
			cerr << "ERROR::In function add_connection, you wanted to add a connection gene with an innovation that already exists" << endl;	
			return;
		}
		else
			Lconnection_genes[innovation].c_g(innovation,in,out,weight,true);

	this->Lnode_genes.at(in).increase_incoming_connection();

	this->update_connection_loop_vector();	
}

void Genome::add_node(int node, gene_type type)
{

	int list_size(Lnode_genes.size());

	if(node >= list_size)
	{
		node_gene missing_node_gene;
		node_gene new_node;

		missing_node_gene.n_g(false);
		new_node.n_g(node, type);

		for (int i = 0; i < node - list_size; ++i)
			Lnode_genes.push_back(missing_node_gene);

		this->Lnode_genes.push_back(new_node);
	}
	else
		if(this->Lnode_genes[node].exist)
		{
			cerr << "ERROR::In function add_node , you wanted to add a node gene with a node number that already exists" << endl;	
			return;
		}
		else
			this->Lnode_genes[node].n_g(node, type);

	if(type == INPUT)
		this->inputs.push_back(node);
	else if(type == OUTPUT)
		this->outputs.push_back(node);
}

bool Genome::is_finished()
{
	for (int i = 0; i < (int)this->outputs.size(); i++)
		if(this->Lnode_genes.at(this->outputs.at(i)).is_ready() == false) return false;
	
	return true;
}
//================================================

void Genome::update_connection_loop_vector()
{
	this->loop_connections.clear();

	for (int i = 0; i < (int)this->input_nodes.size(); i++)
		this->check_loops(this->input_nodes.at(i));
}

bool Genome::check_loops(int node)
{
	if(this->Lnode_genes.is_visited()) return true; // We've just found a loop
	
	this->Lnode_genes.at(node).set_visited(true);

	vector<int> next_nodes;

	next_nodes = this->get_outgoing_connections(node);

	for (int i = 0; i < (int)next_nodes.size(); i++)
		if(this->check_loops(this->Lconnection_genes.get_in(next_nodes.at(i)))
			this->loop_connections.push_back(i);

	// Reset the visited values
	for (int i = 0; i < (int)next_nodes.size(); i++)
		this->Lconnection_genes.set_visited(false);

	this->Lnode_genes.at(node).set_visited(false);

	return false;
}

vector<int> Genome::spread_final_result(vector<int> nodes)
{
	vector<int> next_nodes;
	vector<int> outgoing_nodes; // chenged to outgoing_connections!!!!

	for (int i = 0; i < (int)nodes.size(); i++)
	{
		// Disable the finished nodes
		if(this->Lnode_genes.at(nodes.at(i)).is_ready()) continue;

		outgoing_nodes = get_outgoing_nodes(nodes.at(i));

		next_nodes.insert(next_nodes.begin(), outgoing_nodes.begin(), outgoing_nodes.end());

		for (int i = 0; i < (int)outgoing_nodes.size(); i++)
			this->Lnode_genes.at(outgoing_nodes.at(i)).eval(this->Lnode_genes.at(nodes.at(i)).get_result());	
	}

	// Delete repeated nodes
	sort(next_nodes.begin(), next_nodes.end());
	next_nodes.erase(unique(next_nodes.begin(), next_nodes.end()), next_nodes.end());

	return next_nodes;
}

vector<int> Genome::get_outgoing_connections(int node)
{
	vector<int> outgoing_connections;

	for (int i = 0; i < (int)this->Lconnection_genes.size(); i++)
		if(this->Lconnection_genes.at(i).do_exist() && \
			this->Lconnection_genes.at(i).is_enable() && \
			this->Lconnection_genes.at(i).get_out() == node)
			outgoing_connections.push_back(i);
			//outgoing_connections.push_back(this->Lconnection_genes.at(i).get_in());
	
	return outgoing_connections;
}

//================================================
vector<double> Genome::eval(vector<double> inputs)
{
	if(inputs.size() != this->inputs.size())
	{
		int vector_size;

		vector<int> current_nodes;
		current_nodes = this->input_nodes;

		//this->current_evaluation = this->inputs;

		while(!this->is_finished())
		{
			//current_nodes.clear();

			//vector_size = (int)this->current_evaluation.size();

			// Evaluates all the nodes and return the next nodes to be evaluated in the next loop
			current_nodes = this->spread_final_result(current_nodes);

			for (int i = (int)this->current_nodes.size(); i > 0; i--)
			{
				//current_nodes = this->get_current_nodes(this->current_evaluation.at(0));
				//this->Lnode_genes.at(this->inputs.at(i)).eval(inputs.at(i));

				//this->spread_final_result(current_nodes, this->Lnode_genes.at(this->current_evaluation.at(0)).get_result());
				

				this->get_next_nodes_ready(current_nodes);

				this->current_evaluation.erase(this->current_evaluation.begin());					
			}			
		}
	}
	else
	{
		cerr << "ERROR: In function 'Genome::eval' the number of input values differ from the number of input nodes." << endl;
	}
}

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
		if(Lnode_genes[i].exist)
			o << "\t\t\t{\"exist\": " << Lnode_genes[i].exist  << ",\"node\": " <<Lnode_genes[i].node << ",\"type\": " << Lnode_genes[i].type;
		else
			o << "\t\t\t{\"exist\": " << Lnode_genes[i].exist ;
		

		if(i<node_size-1)
			o <<  "},\n";
		else
			o <<  "}\n";
		
	}

	o << "\t\t],\n\t\t\"connection_genes\":\n\t\t[\n";
	for (int i = 0; i < connection_size; ++i)
	{
		if(Lconnection_genes[i].exist)
			o << "\t\t\t{\"exist\": " << Lconnection_genes[i].exist << ",\"innovation\": " << Lconnection_genes[i].innovation << ",\"in\": " << Lconnection_genes[i].in << ",\"out\": " << Lconnection_genes[i].out << ",\"weight\": " << Lconnection_genes[i].weight << ",\"enable\": " << Lconnection_genes[i].enable;
		else
			o << "\t\t\t{\"exist\": " << Lconnection_genes[i].exist;
		
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
						Nnew_node.n_g(node, (gene_type) type);
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