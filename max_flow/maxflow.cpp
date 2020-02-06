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
	long int flow;
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
	long int id;
	
public:
	std::map<long int, Edge*> edges;
	Node(long int n) {
		id = n;
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
	long int flow;
	std::map<long int, Node*> nodes;
	long int num_of_edges;
	long int num_of_nodes;
public:
	MaxFlow() {
		flow = 0;
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

  bool dfs(long int u, std::vector<long int>& path, std::map<long int, bool>& visited) {
		// for (Edge* e : nodes[u]->edges) {
		for (auto itr = nodes[u]->edges.begin(); itr != nodes[u]->edges.end(); itr++) {
			auto e = itr->second;
			// std::cout << e->str() << std::endl;
			long int v = e->v;
			if (v == t && e->capacity > 0) {
				path.push_back(v);
				return true;
			} else {
				if (visited.find(v) == visited.end() && e->capacity > 0) {
					visited[v] = true;
					path.push_back(v);
					bool res = dfs(v, path, visited);
					if (res) return true;
					path.pop_back();
				}
			}
		}
		return false;
	}

	bool findPath(std::vector<long int>& path) {
		path.push_back(s);
		std::map<long int, bool> visited;
		visited[s] = true;
		bool res = dfs(s, path, visited);

		if (res) {
			// std::cout << "Path: ";
			// for (long int v : path) {
			// 	std::cout << v << "->";
			// }
			// std::cout << "(END)" << std::endl;
			return true;
		}
		return false;
	}

	long int findPathFlow(std::vector<long int>& path) {
		long int path_cap = LONG_MAX;
		for (long int i = 0; i < path.size()-1; i++) {
			long int u = path[i];
			long int v = path[i+1];
			long int cap = nodes[u]->findCap(v);
			path_cap = (path_cap > cap) ? cap : path_cap;
			// std::cout << cap << ":";
		}
		// std::cout << std::endl;
		return path_cap;
	}

	void updateGraphCap(std::vector<long int>& path, long int flow) {
		for (long int i = 0; i < path.size()-1; i++) {
			long int u = path[i];
			long int v = path[i+1];
			nodes[v]->updateCap(u, flow);
			nodes[u]->updateCap(v, -flow);
		} 
	}

	void run(long int val) {
		long int max_flow = 0;
		std::vector<long int> path = {};
		while (findPath(path)) {
			long int flow = findPathFlow(path);
			if (flow < 0) break;
			// std::cout << "Find a path with flow: " << flow << std::endl;
			updateGraphCap(path, flow);
			max_flow += flow;
			path = {};
			// std::cout << "[Running] Max Flow: " << max_flow << std::endl;
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