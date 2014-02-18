
#ifndef CPPN_HH
#define CPPN_HH

#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstrings>
#include "function.hpp"

using namespace std;

namespace ANN_USM
{
	enum gene_type{
		INPUT,
		HIDDEN,
		OUTPUT
	}

	/**********************************************************************************************************************
		Connection
	***********************************************************************************************************************/

	class connection_gene
	{
		public:

			void c_g(int innovation, int in, int out, double weight, bool enable); // fill a conection gene
			void c_g(bool exist);
			void set_visited();

			int get_out();
			int get_in();

			bool is_enable();
			bool do_exist();
			bool is_visited();

		private:

			//void reset_visited();

			int innovation;
			int in;
			int out;

			bool enable;
			bool exist; 
			bool visited;				// Used to see the loops of the graph

			double weight;
	};

	/**********************************************************************************************************************
		Node
	***********************************************************************************************************************/

	class node_gene
	{
		public:

			node_gene();

			void n_g(int node, gene_type type); // fill a node gene
			void n_g(bool exist);
			void increase_incoming_connection();
			void eval(double value);
			void set_visited();

			double get_result();

			bool is_ready();
			bool do_exist();
			bool is_visited();

		private:

			int node;
			int incoming_connections;
			int counter;

			bool exist;
			bool visited;				// Used to see the loops of the graph

			double accumulative_result;
			double final_result;

			gene_type type;

			Function * function;
	};

	/**********************************************************************************************************************
		CPPN
	***********************************************************************************************************************/

	class Genome
	{
		public:

			void add_node(int node, gene_type type);
			void add_connection(int innovation, int in, int out, double weight);
			void change_weight(int innovation, double weight);

			vector<double> eval(vector<double> inputs);

			void save(char path[]); // save to a file
			void load(char path[]); // Load an genetic_encoding from a file.

		private:

			vector <connection_gene> Lconnection_genes; //List of connections genes
			vector <node_gene> Lnode_genes;

			string JSON();

			bool is_finished();

			int spread_final_result(vector<int> next_nodes, double result);

			vector<int> get_outgoing_connections(int node);

			//void get_next_nodes_ready(vector<int> next_nodes);
			void update_connection_loop_vector();
			void check_loops(int node);			// Recursive function that check the existence of loops 

			vector<int> input_nodes;			// Input nodes
			vector<int> output_nodes;			// Output nodes
			//vector<int> current_evaluation;		// Current evaluation
			vector<int> loop_connections;		// Connections that make a loop
	};
}

#endif