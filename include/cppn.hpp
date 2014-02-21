
#ifndef CPPN_HH
#define CPPN_HH

#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include "function.hpp"

using namespace std;

namespace ANN_USM
{
	enum gene_type{
		INPUT,
		HIDDEN,
		OUTPUT
	};

	/**********************************************************************************************************************
		Connection
	***********************************************************************************************************************/

	class connection_gene
	{
		public:

			// in of the connection
			// out of the connection
			
			void c_g(int innovation, int in, int out, double weight, bool enable); // fill a conection gene
			void c_g(bool exist);

			int get_out();
			int get_in();
			int get_innovation();

			double get_weight();

			bool is_enable();
			bool do_exist();

		private:

			int innovation;
			int in;
			int out;

			bool enable;
			bool exist; 

			double weight;
	};

	/**********************************************************************************************************************
		Node
	***********************************************************************************************************************/

	class node_gene
	{
		public:

			node_gene();

			void n_g(int node, string function, gene_type type);
			void n_g(bool exist);
			void increase_incoming_connection();
			void eval(double value);
			void increase_accumulative_result(double value);
			void reset_counter();

			double get_final_result();

			bool is_ready();
			bool do_exist();

			int get_node();
			gene_type get_type();

		private:

			int node;
			int incoming_connections;
			int counter;

			bool exist;

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

			void add_node(int node, string function, gene_type type);
			void add_connection(int innovation, int in, int out, double weight);
			void change_weight(int innovation, double weight);

			vector<double> eval(vector<double> inputs);

			// Save the current configuration into a file
			void save(char path[]);

			// Load a previous configuration from a file
			void load(char path[]);

			string JSON();

		private:

			vector <connection_gene> Lconnection_genes; // List of connections genes
			vector <node_gene> Lnode_genes;				// List of node genes

			//bool is_finished();

			void spread_final_result(int node, double value);

			vector<connection_gene> get_outgoing_connections(int node);

			vector<int> input_nodes;			// Input nodes
			vector<int> output_nodes;			// Output nodes
	};
}

#endif