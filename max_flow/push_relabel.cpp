#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

class Edge
{
public:
	long int u;
	long int v;
	long int capacity;
	Edge(long int u_id, long int v_id, long int cap) {
		u = u_id;
		v = v_id;
		capacity = cap;
	}
	std::string str() {
		std::string ret = "[" + std::to_string(u) + "->" + std::to_string(v) + "]";
		return ret;
	}
	~Edge() {};
};

class Node {
public:
	long int id;
	long int excess;
	long int label;
	std::map<long int, Edge*> edges;
	Node(long int n) {
		id = n;
		excess = 0;
		label = 0;
	}
	void addEdge(long int v, long int edge_cap) {
		edges[v] = new Edge(id, v, edge_cap);
	}
	long int findCap(long int v) {
		if (edges.find(v) != edges.end()) {
			return edges[v]->capacity;
		}
		return -1;
	}
	void updateCap(long int v, long int amount) {
		bool updated = false;
		if (edges.find(v) == edges.end()) {
			if (amount > 0) addEdge(v, amount);
		} else {
			edges[v]->capacity += amount;
		}
	}
	~Node() {};
};

class MaxFlow
{
	long int s;
	long int t;
	std::map<long int, Node*> nodes;
	long int num_of_edges;
	long int num_of_nodes;
public:
	MaxFlow() {
	}

	void readFile(std::ifstream& stream) {
		stream >> num_of_nodes >> num_of_edges;
		stream >> s >> t;
		for (long int i = 0; i < num_of_edges; i++) {
			long int u, v, capacity;
			stream >> u >> v >> capacity;
			// std::cout << u << "," << v << "," << capacity << std::endl;
			if (nodes.find(u) == nodes.end()) {
				nodes[u] = new Node(u);
			}
			if (nodes.find(v) == nodes.end()) {
				nodes[v] = new Node(v);
			}
			nodes[u]->addEdge(v, capacity);
		}
		std::cout << "Create a graph with " << num_of_nodes
						  << " nodes and " << num_of_edges << " edges." << std::endl; 
	}

	bool hasEdge(Node* node) {
		for (auto itr = node->edges.begin(); itr != node->edges.end(); itr++) {
			if (itr->second->capacity > 0) {
				// std::cout << node->id << "->" 
									// << itr->first << " with " << node->excess << std::endl;
				return true;
			}
		}
		return false;
	}

	bool checkNodes(long int& id) {
		for (auto itr = nodes.begin(); itr != nodes.end(); itr++) {
			int node_id = itr->first;
			auto node = itr->second;
			if (node_id == t) continue;
			if (node->excess > 0 && hasEdge(node)) {
				id = node_id;
				// std::cout << "Pick " << id << " with " << node->excess << std::endl;
				return true;
			}
		}
		return false;
	}

	bool findPushNode(long int u_id, long int& v_id) {
		auto node = nodes[u_id];
		long int u_label = node->label;

		long int min_label = LONG_MAX;

		for (auto itr = node->edges.begin(); itr != node->edges.end(); itr++) {
			auto id = itr->first;
			// std::cout << "[" << u_id << "]: " << node->label
			// 					<< "   [" << v_id << "]: " << nodes[v_id]->label
			// 					<< " cap:" << node->edges[itr->first]->capacity << std::endl;
			// cap <= 0 means there is no edge
			if (itr->second->capacity <= 0) 
				continue;
			// std::cout << "[" << u_id << "]: " << node->label
			// 					<< "   [" << id << "]: " << nodes[id]->label << std::endl;
			if (nodes[id]->label+1 <= node->label) {
				if (min_label > nodes[id]->label) {
					min_label = nodes[id]->label;
					v_id = id;
				}
			}
		}

		// std::cout << std::endl;
		return (min_label != LONG_MAX);
	}

	long int push(long int u_id, long int v_id) {
		auto u_excess = nodes[u_id]->excess;
		auto v_cap = nodes[u_id]->edges[v_id]->capacity;

		long int flow = std::min(u_excess, v_cap);

		if (v_id != s) nodes[v_id]->excess += flow;
		nodes[u_id]->excess -= flow;

		nodes[u_id]->updateCap(v_id, -flow);
		nodes[v_id]->updateCap(u_id, flow);

		return flow;
	}

	void relabel(long int u_id) {
		auto node = nodes[u_id];
		long int label = LONG_MAX;

		// for (long int i = 0; i < node->edges.size(); i++) {
		for (auto itr = node->edges.begin(); itr != node->edges.end(); itr++) {
			long int v_label = nodes[itr->first]->label;
			if (itr->second->capacity > 0)
				label = std::min(v_label, label);
		}

		nodes[u_id]->label = label+1;
		// std::cout << "[Relabel]: " << u_id << " to be " << label+1 << std::endl;
		return;
	}

	void init() {
		auto node = nodes[s];
		nodes[s]->label = num_of_nodes;

		for (auto itr = node->edges.begin(); itr != node->edges.end(); itr++) {
			long int cap = node->edges[itr->first]->capacity;
			nodes[itr->first]->excess = cap;
			nodes[itr->first]->updateCap(s,  cap);
			nodes[s]->updateCap(itr->first, -cap);
		}
	}

	void run(long int val) {
		
		init();

		long int max_flow = 0;
		long int u_id;

		while (checkNodes(u_id)) {
			long int v_id;
			long int flow = 0;
			bool pushable = findPushNode(u_id, v_id);
			if (pushable) {
				flow = push(u_id, v_id);
				// std::cout << "Push: [" << u_id << "->" << v_id 
				// 					<< "] with flow " << flow << std::endl; 
			} else {
				relabel(u_id);
			}
			if (v_id == t) {
				max_flow += flow;
				std::cout << "[Running] Max Flow: " << max_flow << std::endl;
			}
			if (max_flow > val) {
				std::cout << "[ERROR]: incorrect result! should stop now!" << std::endl;
				exit(-1);
			}
		}
		std::cout << "Max Flow: " << max_flow << std::endl;
	}
	~MaxFlow() {};
	
};

std::vector<MaxFlow*> readInput(const char * filename) {
	std::ifstream stream(filename);
	long int num_of_problem = 0;
	stream >> num_of_problem;
	std::cout << "Start: MaxFlow with "
						<<  num_of_problem << " problems" << std::endl;

	std::vector<MaxFlow*> vec;

	for (long int i = 0; i < num_of_problem; i++) {
		MaxFlow* max_flow = new MaxFlow();
		max_flow->readFile(stream);
		vec.push_back(max_flow);
	}
	stream.close();

	return std::move(vec);
}

int main(int argc, char const *argv[])
{
	std::vector<MaxFlow*> problem_set = readInput(argv[1]);
	long int val = atol(argv[2]);

	for (MaxFlow* prob : problem_set) {
		prob->run(val);
	}

	return 0;
}