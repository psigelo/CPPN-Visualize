
#ifndef CPPN_HH
#define CPPN_HH

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include "function.hpp"

using namespace std;

namespace ANN_USM
{

	class Connection;
	class Node;
	class CPPN;

	/**********************************************************************************************************************
		Connection
	***********************************************************************************************************************/

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

	/**********************************************************************************************************************
		Node
	***********************************************************************************************************************/

	class Node
	{
		public:

			Node(string,int);
			
			void add_connection(Node*,float);
			void increase_incoming_connection();
			void eval(float);
			void reset();

			// GETTERS
			int get_id();
			float get_result();
			string get_function_name();
			Node * get_next_node();

			// SETTERS
			void set_next_node(Node*);

			// ONLY FOR DEBUGGING PURPOSE
			void print();

		private:

			Node * next_node;					// Next node of the chain

			Connection * head_connection;		// Pointer to the first connection (of the current node)
			Connection * tail_connection;		// Pointer to the last connection (of the current node)
			Connection * nav_connection;		// Multipurpose navigation pointer

			Function * function;				// Function object

			int num_connections;				// Number of connections
			int id; 							// Node id
			int counter;						// Counter used in the evaluation to see if all incoming connections have been done 
			int incoming_connections;			// Number of incoming connections
			float result;						// Accumulative results of node evaluations
	};

	/**********************************************************************************************************************
		CPPN
	***********************************************************************************************************************/

	class CPPN
	{
		public:

			CPPN(bool,string);

			void add_node(string);
			void add_connection(int,int,float);
			void eval();

			// SETTERS
			void set_input(int);
			void set_output(int);
			void set_resolution(int);
			//void set_file_name(string);
			void set_cartesian_constraints(float,float);

			// ONLY FOR DEBUGGING PURPOSE
			void print();

		private:

			void reset();

			int num_nodes;			// Number of nodes
			bool g_flag;			// Graphic flag

			string file_name;		// File name

			Node * head_node;		// Pointer to the first node
			Node * tail_node;		// Pointer to the last node
			Node * nav_node;		// Multipurpose navigator pointer

			vector<float> max;		// Cartesian constraints of the multidimensional grid
			vector<float> min;					
			vector<int> resolution;	// Resolution of the multidimensional grid
			vector<int> inputs;		// Input nodes
			vector<int> outputs;	// Output nodes
	};

}

#endif