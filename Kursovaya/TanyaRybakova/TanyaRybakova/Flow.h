#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include "Map.h"
using namespace std;
class Flow {
private:
	int** edge, **capacity, **adj;
	int indS, indT, maxFlowFormat, maxFlow, N;
	void readInfo(string);
	void fillInd(string);
	int findAugPath(int*);
	Map<string, int> indices;
	Map<int, string> names;
public:
	Flow(string);
	~Flow();
	int getMaxFlow();
};

Flow::~Flow()
{
	for (size_t i = 0; i < N; i++)
	{
		delete[] adj[i];
		delete[] capacity[i];
		delete[] edge[i];
	}
}

Flow::Flow(string filename)
{
	maxFlowFormat = INT_MIN;
	maxFlow = -1;
	fillInd(filename);
	edge = new int* [N];
	adj = new int* [N];
	capacity = new int* [N];
	for (int i = 0; i < N; i++) {
		edge[i] = new int[N];
		adj[i] = new int[N];
		capacity[i] = new int[N];
		for (int j = 0; j < N; j++) {
			adj[i][j] = 0;
			capacity[i][j] = 0;
			edge[i][j] = 0;
		}
	}
	readInfo(filename);
}

int Flow::getMaxFlow() {
	if (this->maxFlow != -1)
	{
		return maxFlow;
	}
	int flow = 0;
	int* parent = new int[N];
	int aug_flow = findAugPath(parent);
	while (aug_flow) {
		flow += aug_flow;
		int current = indT;
		while (current != indS) {
			int prev = parent[current];
			capacity[prev][current] -= aug_flow;
			capacity[current][prev] += aug_flow;
			edge[prev][current] += aug_flow;
			maxFlowFormat = edge[prev][current] > maxFlowFormat ? edge[prev][current] : maxFlowFormat;
			current = prev;
		}
		aug_flow = findAugPath(parent);
	}
	this->maxFlow = flow;
	return flow;
}

void Flow::readInfo(string filename) {
	ifstream file(filename);
	if (!file.good())
		throw exception("Can't read this file");
	for (string line; getline(file, line);) {
		istringstream iss(line);
		string u, v, c_s;
		getline(iss, u, ' ');
		getline(iss, v, ' ');
		getline(iss, c_s, ' ');
		int c = std::stoi(c_s);
		if (c <= 0)
			throw exception("Capacity can only be a positive integer");
		auto u_i = indices.find(u);
		auto v_i = indices.find(v);
		if (capacity[u_i][v_i] != 0)
			throw exception("File contains two lines describing one edge");
		capacity[u_i][v_i] = c;
		adj[u_i][v_i] = v_i;
		adj[v_i][u_i] = u_i;
	}
	file.close();
}

void Flow::fillInd(string filename) {
	ifstream file(filename);
	if (!file.good())
		throw exception("Can't read this file");
	int max_index = 0;
	for (string line; getline(file, line);) {
		istringstream iss(line);
		string u, v, c_s;
		getline(iss, u, ' ');
		getline(iss, v, ' ');
		getline(iss, c_s, ' ');
		try {
			indices[u];
		}
		catch (...) {
			indices.insert(u, max_index);
			names.insert(max_index, u);
			if (u == "S")
				indS = max_index;
			else if (u == "T")
				indT = max_index;
			max_index++;
		}
		try {
			indices[v];
		}
		catch (...) {
			indices.insert(v, max_index);
			names.insert(max_index, v);
			if (v == "S")
				indS = max_index;
			else if (v == "T")
				indT = max_index;
			max_index++;
		}
	}
	file.close();
	try {
		indices["S"];
		indices["T"];
	}
	catch (...) {
		throw exception("File does not contain a source or sink (or both)");
	}
	N = max_index;
}

int Flow::findAugPath(int* parent) {
	for (size_t i = 0; i < N; i++)
		parent[i] = -1;
	Queue<pair<int, int>> queue;
	queue.enqueue(make_pair(indS, INT_MAX));
	while (!queue.isEmpty()) {
		int current = queue.front().first;
		int flow = queue.front().second;
		queue.dequeue();
		for (int i = 0; i < N; i++) {
			int next = adj[current][i];
			if (next == -1)
				continue;
			if (parent[next] == -1 && capacity[current][next] != 0) {
				parent[next] = current;
				int aug_flow = flow < capacity[current][next] ? flow : capacity[current][next];
				if (next == indT)
					return aug_flow;
				queue.enqueue({ next, aug_flow });
			}
		}
	}
	return 0;
}
