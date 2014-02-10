
#include "cppn.hpp"

using namespace ANN_USM;

/***********************
		Connection
************************/

Connection::Connection(Node * node, float weight)
{
	this->connected_node = node;
	node->increase_incoming_connection();
	this->weight = weight;
	this->next_connection = NULL;
}

// GETTERS

Node * Connection::get_connected_node() 
{ 
	return this->connected_node; 
}

int Connection::get_connected_node_id() 
{ 
	return this->connected_node->get_id(); 
}

float Connection::get_weight() 
{ 
	return this->weight; 
}

Connection * Connection::get_next_connection() 
{ 
	return this->next_connection; 
}

// SETTERS

void Connection::set_next_connection(Connection * connection) 
{ 
	this->next_connection = connection; 
}

/*******************
		Node
********************/

Node::Node(string function, int id)
{
	this->function = new Function(function);
	this->id = id;
	this->counter = 0;
	this->incoming_connections = 0;
	this->num_connections = 0;
	this->next_node = NULL;
}

void Node::add_connection(Node * node, float weight)
{
	nav_connection = new Connection(node, weight);

	if(num_connections == 0)
	{
		head_connection = tail_connection = nav_connection;
	}
	else
	{
		tail_connection->set_next_connection(nav_connection);
		tail_connection = nav_connection;
	}

	num_connections++;
}

void Node::increase_incoming_connection() 
{ 
	this->incoming_connections++; 
}

void Node::eval(float value)
{
	//cout << "node: " << this->get_id() << " | value: " << value << " | result: " << result;
	result += value;
	counter++;
	//cout << " | result: " << result << endl;

	if(incoming_connections == counter)
	{
		result = this->function->eval(result);

		Node * aux_node;
		nav_connection = head_connection;
		for (int i = 0; i < num_connections; i++)
		{
			aux_node = nav_connection->get_connected_node();
			aux_node->eval(result * nav_connection->get_weight());
			nav_connection = nav_connection->get_next_connection();
		}
	}
}

void Node::reset() 
{ 
	counter = 0; 
	result = 0; 
}

// ONLY FOR DEBUGGING PURPOSE
void Node::print()
{	
	nav_connection = head_connection;
	cout << "Node: " << this->get_id() << " | Function: " << this->get_function_name() << endl << "   ";
	for (int i = 0; i < num_connections; i++)
	{
		cout << "(" << nav_connection->get_connected_node_id() << "," << nav_connection->get_weight() << ") ";
		nav_connection = nav_connection->get_next_connection();
	}
	cout << endl;
}

// GETTERS

int Node::get_id() 
{ 
	return id; 
}

float Node::get_result() 
{ 
	return result; 
}

string Node::get_function_name() 
{ 
	return this->function->get_name(); 
}

Node * Node::get_next_node() 
{ 
	return this->next_node; 
}

// SETTERS

void Node::set_next_node(Node * node) 
{ 
	this->next_node = node; 
}

/*******************
		CPPN
********************/

CPPN::CPPN()
{
	// Default values
	x_max = y_max = 1;
	x_min = y_min = -1;
	x_res = y_res = 3;
	num_nodes = 0;
	file_name = "cppn-out";
}

void CPPN::add_node(string function)
{
	//if(function >= FUNCTION_NUM || function < 0) return;

	nav_node = new Node(function, num_nodes);

	if(num_nodes == 0)
	{
		head_node = tail_node = nav_node;
	}
	else
	{
		tail_node->set_next_node(nav_node);
		tail_node = nav_node;
	}

	num_nodes++;
}

void CPPN::add_connection(int node_1, int node_2, float weight)
{
	if(num_nodes <= node_1 || num_nodes <= node_2) 
	{
		cout << "error at CPPN::add_connection(): one or both of the specified nodes don't exist" << endl;
		return;
	}

	Node * aux_1;
	Node * aux_2;

	aux_1 = head_node;
	aux_2 = head_node;

	while(node_1-- > 0) aux_1 = aux_1->get_next_node();
	while(node_2-- > 0) aux_2 = aux_2->get_next_node();

	aux_1->add_connection(aux_2, weight);
}

void CPPN::eval()
{
	ofstream file;
	file.open (file_name.c_str());

	Node * aux_x;
	Node * aux_y;
	Node * aux_o;

	aux_x = head_node;
	aux_y = head_node;
	aux_o = head_node;

	for (int i = 0; i < x_input; i++) aux_x = aux_x->get_next_node();
	for (int i = 0; i < y_input; i++) aux_y = aux_y->get_next_node();
	for (int i = 0; i < output; i++) aux_o = aux_o->get_next_node();

	float x_unit = abs(x_max-x_min)/(x_res-1);
	float y_unit = abs(y_max-y_min)/(y_res-1);

	// Creates the grid of results
	file << x_res << endl << y_res << endl;
	for (float y = y_max; y >= y_min; y -= y_unit)
	{
		for (float x = x_min; x <= x_max; x += x_unit)
		{
			this->reset();

			aux_x->eval(x);
			aux_y->eval(y);		

			file << aux_o->get_result() << "\t";
			//cout << "(" << x << "," << y << ")\t--CPPN--> " << aux_o->get_result() << endl;
		}
		file << endl;
	}

	file.close();
}

// SETTERS

void CPPN::set_input(int x_node, int y_node)
{
	if(num_nodes <= x_node || num_nodes <= y_node)
	{
		cout << "error at CPPN::set_input(): one or both of the specified nodes don't exist" << endl;
		return;
	}

	this->x_input = x_node;
	this->y_input = y_node;

	Node * aux_x;
	Node * aux_y;

	aux_x = head_node;
	aux_y = head_node;

	for (int i = 0; i < x_input; i++) aux_x = aux_x->get_next_node();
	for (int i = 0; i < y_input; i++) aux_y = aux_y->get_next_node();

	aux_x->increase_incoming_connection();
	aux_y->increase_incoming_connection();
}

void CPPN::set_output(int node)
{
	if(num_nodes <= node)
	{
		cout << "error at CPPN::set_output(): the specified node doesn't exist" << endl;
		return;
	}

	this->output = node;
}

void CPPN::set_resolution(int x_res, int y_res)
{
	this->x_res = x_res;
	this->y_res = y_res;
}

void CPPN::set_file_name(string file_name)
{
	this->file_name = file_name;
}

void CPPN::set_cartesian_constraints(float x_max, float x_min, float y_max, float y_min)
{
	this->x_max = x_max;
	this->x_min = x_min;
	this->y_max = y_max;
	this->y_min = y_min;
}

void CPPN::reset() 
{
	nav_node = head_node;
	for (int i = 0; i < num_nodes; i++)
	{
		nav_node->reset();
		nav_node = nav_node->get_next_node();
	}
}

// ONLY FOR DEBUGGING PURPOSE
void CPPN::print()
{
	nav_node = head_node;
	for (int i = 0; i < num_nodes; i++) 
	{
		nav_node->print();
		nav_node = nav_node->get_next_node();
	}
}