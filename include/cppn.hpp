
#ifndef CPPN_HH
#define CPPN_HH

#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include "function.hpp"

using namespace std;

class Connection;
class Node;
class CPPN;

/***********************
		Connection
************************/

class Connection
{
	public:

		Connection(Node*,float);

		// GETTERS
		float get_weight();
		int get_connected_node_id();
		Connection * get_next_connection();
		Node * get_connected_node();

		// SETTERS
		void set_next_connection(Connection*);

	private:

		Connection * next_connection;
		Node * connected_node;

		float weight;
};

/*******************
		Node
********************/

class Node
{
	public:

		Node(function_t,int);
		void add_connection(Node*,float);
		void increase_incoming_connection();
		void eval(float);
		void reset();

		// ONLY FOR DEBUGGING PURPOSE
		void print();

		// GETTERS
		int get_id();
		float get_result();
		string get_function_name();
		Node * get_next_node();

		// SETTERS
		void set_next_node(Node*);

	private:

		Node * next_node;

		Connection * head_connection;
		Connection * tail_connection;
		Connection * nav_connection;

		Function * function;

		int num_connections;
		int id; 							// Node id
		int counter;						// Counter used in the evaluation to see if all incoming connections are ready 
		int incoming_connections;			// Number of "indirect" connections
		float result;						// Final result of evaluation
};

/*******************
		CPPN
********************/

class CPPN
{
	public:

		CPPN();

		void add_node(function_t);
		void add_connection(int,int,float);
		void eval();

		// SETTERS
		void set_input(int,int);
		void set_output(int);
		void set_resolution(int,int);
		void set_file_name(string);
		void set_cartesian_constraints(float,float,float,float);

		// ONLY FOR DEBUGGING PURPOSE
		void print();

	private:

		void reset();

		Node * head_node;
		Node * tail_node;
		Node * nav_node;

		int num_nodes;

		float x_max, x_min, y_max, y_min;	// Cartesian constraints of the grid
		int x_res, y_res;					// Resolution of the grid
		int x_input, y_input;				// Input nodes
		int output;							// Output node
		string file_name;
};

#endif