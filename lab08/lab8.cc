#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <utility>
using namespace std;

class GenericGraph {
	public:
		GenericGraph(int numVertices, int numEdges) {																// Initializes vectors of type Vertex and Edge
			V.resize(numVertices);
			E.resize(numEdges);
		}

		int vertex (const string & name) {																			// Initializes value into vertex
			// cout << V.size();
			// int k;
			// cin >> k;
			for (int i = 0; i < V.size(); ++i) {
				if (V.at(i) == NULL) {
					V.at(i) = new Vertex;
					V.at(i)->name = name;
					return 1;
				}
				if (name == V.at(i)->name) return -1;
			}
			return -2;
		}
		void print () {
			for (auto n : V) {
				if (n == NULL) {
					cout << "null\n";
				}
				else {
					cout << n->name << endl;
				}
			}
		}

		int edge (const string & name, double weight, const string & v1, const string & v2) {						// Initializes edges
			int counter = 0;
			int vPoint1 = 0;
			int vPoint2 = 0;
			for (int i = 0; i < V.size(); ++i) {																	// Checks for existence of both vertices
				if (v1 == V.at(i)->name) {
					++counter;
					vPoint1 = i;																					// Finds index of vertex 1
				}
				if (v2 == V.at(i)->name) {
					++counter;
					vPoint2 = i;																					// Finds index of vertex 2 (could be implemented with find func)
				}
			}

			if (counter != 2) return -3;																			// Returns -3 if either vertices inputted do not exist
			
			for (int i = 0; i < E.size(); ++i) {																	// 
				if (E.at(i) == NULL) {
					E.at(i) = new Edge;
					E.at(i)->name = name;
					E.at(i)->weight = weight;
					E.at(i)->endpoints = make_pair(vPoint1,vPoint2);												// Makes pair of next two vertices
					E.at(i)->selected = true;
					
					V.at(vPoint1)->edges.push_back(i);
					V.at(vPoint2)->edges.push_back(i);
					return 1;
				}
				else if (name == E.at(i)->name) return -1;															// Returns -1 if name is already in another edge
				else if (E.at(i)->endpoints == make_pair(vPoint1,vPoint2)) {
					return -4;
				}
			}
			return -2;
		}

		bool valid () {
			for (int i = 0; i < V.size(); ++i) {
				if (V.at(i) == NULL) return false;
			}
			for (int j = 0; j < E.size(); ++j) {
				if (E.at(j) == NULL) return false;
			}
			return true;
		}																											//
//------------------------------------------------------------
// Union/Find Algorithm
//------------------------------------------------------------
		// vector<int> id;																							// Delcares id to check
		vector<int> sz;
		// int size;

		double NUTs (vector<string> & edgeList) {																	
			for (int i = 0; i < V.size(); ++i) {
				disjointSets.push_back(i);																			// Creates disjoint sets vector of size |V|

				sz.push_back(i);																					// Initializes Union/Find
			}


			for (int j = 0; j < E.size(); ++j) {
				E.at(j)->selected = false;																			// Resets the selected field for every edge to false
			}

			for (int k = 0; k < E.size(); ++k) {
				int u = E.at(k)->endpoints.first;																	// endpoints of edge
				int v = E.at(k)->endpoints.second;
				if (find(u) != find(v)) {
					edgeList.push_back(E.at(k)->name);
					E.at(k)->selected = true;
					merge(u,v);
				}
				if (edgeList.size() == V.size() - 1) break;
			}
			double weight = 0;
			sort (edgeList.begin(),edgeList.end());
			for (int m = 0; m < edgeList.size(); ++m) {
				for (int h = 0; h < E.size(); ++h) {
					if (edgeList.at(m) == E.at(h)->name) {
						weight += E.at(h)->weight;
					}
				}
			}
			return weight;

		}

		void merge (int x, int y) {
			int i = find(x);
			int j = find(y);
			if (i == j) return;																						// If find(a) and find(b) are same, already merged

			if (sz.at(i) < sz.at(j)) {
				disjointSets.at(i) = j;
				sz.at(j) += sz.at(i);
			}
			else {
				disjointSets.at(j) = i;
				sz.at(i) += sz.at(j);
			}
			cnt--;
			return;
		}

		int find (int p) {																							// Finds ID of p (index of object)
			int root = p;
			while (root != disjointSets.at(root)) {
				root = disjointSets.at(root);
			}
			while (p != root) {
				int newP = disjointSets.at(p);
				disjointSets.at(p) = root;
				p = newP;
			}
			return root;
		}

		int treeDetails (const vector<string> & edgeList, vector<vector<string>> & fragments) {
			for ( int i = 0; i < E.size(); ++i) { 												// Sets all to false
				E.at(i)->selected = false;
			}
			for (int k = 0; k < edgeList.size(); ++k) {											// If matches with edgeList, sets selected to true
				for (int m = 0; m < E.size(); ++m) {
					if (edgeList.at(k) == E.at(m)->name) {
						E.at(m)->selected = true;
					}
				}

			vector<string> used;																// Creates vector to keep track of all used edges
			for (int m = 0; m < E.size(); ++m) {
				vector<string> inside;
				vector<string> verticesUsed;
				traversal (m, verticesUsed, inside, used);

				if (!verticesUsed.empty()) {
					sort(verticesUsed.begin(), verticesUsed.end());
					inside.insert(inside.begin(),verticesUsed.at(0));
				}

				if (!inside.empty()) {
					fragments.push_back(inside);
				}
				
			}

			for (int i = 0; i < fragments.size(); ++i) {
				for (int j = i; j < fragments.size(); ++j) {
					if (fragments[i].size() < fragments[j].size()) {
						swap(fragments[i],fragments[j]);
					}
				}
			}
			return fragments.size();
		}

		void traversal (int edgeIndex, vector<string> & verticesUsed, vector<string> & inside, vector<string> & edgesUsed) {				// return number of vertices // index of Vertex  // vector for edges in fragment
			if (E.at(edgeIndex)->selected == true && !exists(edgesUsed, E.at(edgeIndex)->name)) {											// Picks first edge to start with in vector
				E.at(edgeIndex)->selected = false;
				inside.push_back(E.at(edgeIndex)->name);											// Puts edge in vector to be pushed into fragments
				
				edgesUsed.push_back(E.at(edgeIndex)->name);												// Puts edge into vector if ever used to keep track
				
				int v1 = E.at(edgeIndex)->endpoints.first;													// Index of first vertex						
				int v2 = E.at(edgeIndex)->endpoints.second;													// Index of second vertex

				if (!exists(verticesUsed, V.at(v1)->name)) {
					verticesUsed.push_back(V.at(v1)->name);
					for (int i = 0; i < V.at(v1)->edges.size(); ++i) {									// For vertex 2
						int newEdgeIndex = V.at(v1)->edges.at(i);										// Gets new Edge index from vertex
						traversal(newEdgeIndex, verticesUsed, inside, edgesUsed);
					}
				}
				if (!exists(verticesUsed,V.at(v2)->name)) {
					verticesUsed.push_back(V.at(v2)->name);
					for (int i = 0; i < V.at(v2)->edges.size(); ++i) {									// For vertex 1
						int newEdgeIndex = V.at(v2)->edges.at(i);										// Gets new Edge index from vertex	
						traversal(newEdgeIndex, verticesUsed, inside, edgesUsed);
					}
				}
			}
			else {
				return;
			}
		}

		bool exists (vector<string> & inside, string name) {
			for (int i = 0; i < inside.size(); ++i) {
				if (inside.at(i) == name) return true;
			}
			return false;
		}

		void printFragments (vector<vector<string>> & fragments) {
			for (int i = 0; i < fragments.size(); ++i) {
				cout << "Level " << i << ": ";
				for (int k = 0; k < fragments[i].size(); ++k){
					cout << fragments[i].at(k) << ' ';
				}
				cout << endl;
			}
		}

	private:
		struct Vertex {
			string name;
			vector<int> edges; 																						// Holds adjacent edge numbers
		};

		struct Edge {
			string name;
			double weight;
			pair<int,int> endpoints;
			bool selected;
		};
		vector<Vertex*> V;
		vector<Edge*> E;
		vector<int> disjointSets;
		int cnt;

};


int main(int argc, char *argv[]) {
	string myfile;
	string input;
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " NAME" << endl;
	}
	else 
	myfile = argv[1];
	ifstream inputFile;
	inputFile.open(myfile.c_str());

	stringstream ss;
	int vertices = 0;
	int edges = 0;
	string vert;
	vector<string> edgeList;

	if (inputFile.is_open()) {
		getline(inputFile, input);
		ss.clear();
		ss.str("");
		ss << input;
		ss >> vertices >> edges;
		GenericGraph G (vertices, edges);

		for (int i = 0; i < vertices; ++i) {
			getline(inputFile, input);
			vert = input;
			int result = G.vertex(vert);																				// Vertex put into tree, must output results if error occurs
			if (result == -1) {
				cout << "Error: Attempt to reassign same name to another vertex.\n";
			}
			if (result == -2) {
				cout << "Error: Attempt to initialize more than |V| vertices.\n";
			}
		}
		// G.print();
		for (int j = 0; j < edges; ++j) {
			getline(inputFile, input);
			ss.clear();
			ss.str("");
			ss << input;
			string name;
			double weight;
			string vert1;
			string vert2;
			ss >> name >> weight >> vert1 >> vert2;
			// int m;
			// cin >> m;
			int result = G.edge (name, weight, vert1, vert2);
			if (result == -1) {
				cout << "Error: Attempt to reassign same name to another edge.\n";
			}
			if (result == -2) {
				cout << "Error: Attempt to initialize more than |E| edges.\n";
			}
			if (result == -3) {
				cout << "Error: Attempt to connect to non-existent vertex.\n";
			}
			if (result == -4) {
				cout << "Error: Attempt to creat more than one edge connecting hte same pair of vertices.\n";
			}
		}

		double weight = G.NUTs (edgeList);

		ofstream outputFile;
		outputFile.open("graph_ex.out");
		if (outputFile.is_open()) {
			outputFile << "Weight of the tree: " << weight << endl;
			outputFile << "Edges in the tree: ";
			for (int i = 0; i < edgeList.size(); ++i) {
				outputFile << edgeList.at(i) << ' ';
			}
			vector<vector<string>> fragments;

			// int k;
			// cin >> k;

			int frags = G.treeDetails(edgeList, fragments);


			outputFile << "Number of fragments in tree: " << frags << endl;

			for (int k = 0; k < fragments.size(); ++k) {
				outputFile << "Fragment " << k + 1 << " contains vertex " << fragments[k].at(0);
				outputFile << " and the following " << fragments[k].size() - 1 << " edges: ";
				for (int j = 1; j < fragments[k].size(); ++j) {
					outputFile << fragments[k].at(j) << ' ';
				}
				outputFile << endl;
			}
		}
		inputFile.close();
	}

	

	return 0;
}