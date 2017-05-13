#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <utility>
#include <functional>
using namespace std;

class GenericGraph {
	public:
		GenericGraph(int numVertices, int numEdges) {																// Initializes vectors of type Vertex and Edge
			V.resize(numVertices);
			E.resize(numEdges);
		}

		int vertex (const string & name) {																			// Initializes value into vertex
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

			for (int i = 0; i < E.size(); ++i) {																	
				if (E.at(i) == NULL) {
					E.at(i) = new Edge;
					E.at(i)->name = name;
					E.at(i)->weight = weight;
					E.at(i)->endpoints = make_pair(vPoint1,vPoint2);												// Makes pair of next two vertices
					E.at(i)->selected = false;
					
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

		vector<int> sz;

		void setDisjoints () {
			for (int i = 0; i < V.size(); ++i) {
				disjointSets.push_back(i);																			// Creates disjoint sets vector of size |V|

				sz.push_back(i);																					// Initializes Union/Find
			}
		}

		void debugFunction (string edge, int vertex1, int vertex2) {
			cout << "Edge " << edge << " connects fragments " << V.at(vertex1)->name << " and " << V.at(vertex2)->name << endl;
		}

		void setDebug (string setting) {
			if (setting == "quiet") {
				debug = false;
			}
			if (setting == "debug") {
				debug = true;
			}
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

		double getWeight (vector<string> & edgeList) {
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

		void traversal (int edgeIndex, vector<string> & verticesUsed, vector<string> & inside, vector<string> & edgesUsed) {				// Preorder traversal of graph
			if (E.at(edgeIndex)->selected == true && !exists(edgesUsed, E.at(edgeIndex)->name)) {											// Picks first edge to start with in vector
				E.at(edgeIndex)->selected = false;
				inside.push_back(E.at(edgeIndex)->name);															// Puts edge in vector to be pushed into fragments
				
				edgesUsed.push_back(E.at(edgeIndex)->name);															// Puts edge into vector if ever used to keep track
				
				int v1 = E.at(edgeIndex)->endpoints.first;															// Index of first vertex						
				int v2 = E.at(edgeIndex)->endpoints.second;															// Index of second vertex

				if (!exists(verticesUsed, V.at(v1)->name)) {
					verticesUsed.push_back(V.at(v1)->name);
					for (int i = 0; i < V.at(v1)->edges.size(); ++i) {												// For vertex 2
						int newEdgeIndex = V.at(v1)->edges.at(i);													// Gets new Edge index from vertex
						traversal(newEdgeIndex, verticesUsed, inside, edgesUsed);
					}
				}
				if (!exists(verticesUsed,V.at(v2)->name)) {
					verticesUsed.push_back(V.at(v2)->name);
					for (int i = 0; i < V.at(v2)->edges.size(); ++i) {												// For vertex 1
						int newEdgeIndex = V.at(v2)->edges.at(i);													// Gets new Edge index from vertex	
						traversal(newEdgeIndex, verticesUsed, inside, edgesUsed);
					}
				}
			}
			else {
				return;
			}
		}

		bool exists (vector<string> & inside, string name) {														// Checks vector of strings
			for (int i = 0; i < inside.size(); ++i) {
				if (inside.at(i) == name) return true;
			}
			return false;
		}

		bool exists (vector<int> & inside, int index) {																// Checks vector of ints
			for (int i = 0; i < inside.size(); ++i) {
				if (inside.at(i) == index) return true;
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

		double NUTs (vector<string> & edgeList) {																	
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
					if (debug) {
						debugFunction (E.at(k)->name, u, v);														// Outputs debugger lines
					}
				}
				if (edgeList.size() == V.size() - 1) break;
			}
			return getWeight(edgeList);
		}

		template<class T> using min_heap = priority_queue<T, std::vector<T>, std::greater<T>>;						// Alias for min_heap (min priority queue)
		
		double Kruskal ( vector<string> & edgeList ) {
			for (int j = 0; j < E.size(); ++j) {
				E.at(j)->selected = false;																			// Resets the selected field for every edge to false
			}

			min_heap<pair<double,Edge*>> pq;																		// Priority queue with min heap										
			// Disjoint sets are set implicitly
			
			for (int i = 0; i < E.size(); ++i) {																	// Put all Edges into priority queue
				pq.push(make_pair(E.at(i)->weight,E.at(i)));
			}

			while (edgeList.size() != V.size() - 1 && !pq.empty()) {
				pair<double,Edge*> p = pq.top();
				Edge* e = p.second;
				int u = find(e->endpoints.first);																	// Find checks in disjoint sets which is implicit
				int v = find(e->endpoints.second);
				if (u != v) {
					edgeList.push_back(e->name);
					e->selected = true;
					merge(u, v);
					if (debug) {
						debugFunction (e->name, u, v);																// Outputs debugger lines
					}
				}
				pq.pop();
			}
			return getWeight(edgeList);
		}

		double Prim ( int root, vector<string> & edgeList) {
			for (int j = 0; j < E.size(); ++j) {
				E.at(j)->selected = false;																			// Resets the selected field for every edge to false
			}

			min_heap<pair<double, Edge*>> pq;
			vector<int> verticesUsed;

			verticesUsed.push_back(root);																			// Puts name of vertex into list of vertices used
			for (int i = 0; i < V.at(root)->edges.size(); ++i) {													// Initial edges of vertex added to queue
				int index = V.at(root)->edges.at(i);
				// if (E.at(index)->selected == false) {																// Gets index of each Edge connected to Root
					pq.push(make_pair(E.at(index)->weight, E.at(index)));											// Pushes Edges into Priority Queue
				// }
			}

			while (edgeList.size() != V.size() - 1 && !pq.empty()) {
				pair<double, Edge*> p = pq.top();																	// Takes next edge from queue
				Edge* e = p.second;
				pq.pop();																							// Immediately pops edge from queue so it does not get resorted wh en new edges added
				
				if (!exists(verticesUsed, e->endpoints.first)) {
					root = e->endpoints.first;
					verticesUsed.push_back(root);
					for (int i = 0; i < V.at(root)->edges.size(); ++i) {
						int index = V.at(root)->edges.at(i);
						pq.push(make_pair(E.at(index)->weight, E.at(index)));
					}
				}
				if (!exists(verticesUsed, e->endpoints.second)) {
					root = e->endpoints.second;
					verticesUsed.push_back(root);
					for (int i = 0; i < V.at(root)->edges.size(); ++i) {
						int index = V.at(root)->edges.at(i);
						pq.push(make_pair(E.at(index)->weight, E.at(index)));
					}
				}
				int u = find(e->endpoints.first);
				int v = find(e->endpoints.second);
		
				if (u != v) {
					edgeList.push_back(e->name);
					e->selected = true;
					merge(u, v);
					if (debug) {
						debugFunction (e->name, u, v);																// Outputs debugger lines
					}
				}
			}

			while (!pq.empty()) {
				pair<double, Edge*> p = pq.top();																	// Takes next edge from queue
				Edge* e = p.second;
				pq.pop();		
				e->selected = true;																					// Sets unused edges to checked 						
			}

			return getWeight(edgeList);
		}
		
		double PrimPlus ( vector<string> & edgeList ) {
			for (int k = 0; k < edgeList.size(); ++k) {												// If matches with edgeList, sets selected to true
				for (int m = 0; m < E.size(); ++m) {
					if (edgeList.at(k) == E.at(m)->name) {
						E.at(m)->selected = true;													// Ensures all checked edges are set to selected = true
					}
				}
			}
			int numFrags = 1;																		// Number of fragments kept track of
			while (edgeList.size() != V.size() - numFrags) {										// Continue while not all edges have been checked due to possibility there are no edges but many vertices
				numFrags+= 1;																		// Algorithm: edgeList.size() should be equivalent to V.size() subtract number of fragments 
				int nextRoot = 0;
				for (int i = 0; i < E.size(); ++i) {
					if (E.at(i)->selected == false) {
						int u = find(E.at(i)->endpoints.first);
						int v = find(E.at(i)->endpoints.second);
						if (u != v) {
							nextRoot = u;
							break;
						}
					}
				}
				Prim(nextRoot, edgeList);
			}
			return getWeight(edgeList);
		}
		
		double YAMA ( vector < string > & edgeList ) {
			for (int j = 0; j < E.size(); ++j) {
				E.at(j)->selected = false;															// Resets the selected field for every edge to false
			}

			queue<int> FIFO;
			for (int i = 0; i < V.size() - 1; ++i) {												// Push all integers from 0 to |V|-1 into FIFO for distinct fragments
				FIFO.push(i);
			}

			int frags = 1;

			while (edgeList.size() != V.size() - frags) {
				int p = FIFO.front();
				FIFO.pop();
				while (find(p) != p) {																// Condition: FIFO queue contains the subset of ID's for every surviving fragment
					p = FIFO.front();																// and possibly some dead fragments 
					FIFO.pop();
				}

				min_heap<pair<double, Edge*>> pq;
				for (int q = 0; q < V.size(); ++q) {												// Checks edges of all vertices in each fragment
					if (p == find(q)) {
						for (int i = 0; i < V.at(q)->edges.size(); ++i) {
							Edge* e = E.at(V.at(q)->edges.at(i));
							int u = find(e->endpoints.first);
							int v = find(e->endpoints.second);
							if (!exists(edgeList, e->name) && u != v) {
								pq.push(make_pair(e->weight, E.at(V.at(q)->edges.at(i))));
							}
						}
					}
				}
				
				if (pq.empty()) { 
					frags += 1;
				}
				else {
					Edge* e = pq.top().second;
					pq.pop();

					int u = find(e->endpoints.first);
					int v = find(e->endpoints.second);

					edgeList.push_back(e->name);
					merge(u,v);
					if (debug) {
						debugFunction (e->name, u, v);												// Outputs debugger lines
					}

					if (p == find(p)) {																// If p survives, then p is pushed back into FIFO, otherwise deleted
						FIFO.push(p);
					}
				}
			}
			return getWeight(edgeList);
		}


	private:
		struct Vertex {
			string name;
			vector<int> edges; 																		// Holds adjacent edge numbers
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

		bool debug;

};


int main() {
	stringstream ss;
	string myfile;
	string algorithm;											// Stores algorithm to be executed
	string debug;												// Selects whether to run with or wihtout debug

	cout << "Select Algoirthm [-n (NUTs), -k (Kruskal), -p'root' (Prim), -y (YAMA), -a (all four)]: \n";
	cin >> algorithm;											// User chooses Algorithm:  '-n' -> NUTS
//																						    '-k' -> Kruskal's
//																							'-p#' -> Prim's. [# means 'root'. Ex: p12 starts from vertex 12] 
//																							'-y' -> YAMA
//																							'-a' -> chooses all algorithms, one after another.

	string first_two = algorithm.substr(0,2);					// Takes first two characters of string into substring first_two
	if (algorithm != "-n" && algorithm != "-k" && algorithm != "-y" && algorithm != "-a" && first_two != "-p") {
		cout << "Error: Invalid algorithm selection. \n";
		return 0;
	}

	string rootVertexParse;										// If "-p" selected, create string for numbers, then parse into numbers
	int rootIndex = 0;											// Index of beginning root
	if (first_two == "-p" && algorithm.size() > 2) {
		for (int i = 2; i < algorithm.size(); ++i) {
			rootVertexParse.push_back(algorithm.at(i));
		}
		ss.clear();
		ss.str("");
		ss << rootVertexParse;									// Parses string of index into integer
		ss >> rootIndex;										// Index of vertex stored in rootIndex
	}

	cout << "Turn on/off debug [debug (on), quiet (off): \n";
	cin >> debug;												// User chooses debug option:	debug -> prints line to standard output when alg adds edge to tree
//																								quiet -> debug is off and prints nothing
	if (debug != "debug" && debug != "quiet") {
		cout << "Error: Invalid command for debug selection. Options: debug, quiet. \n";
		return 0;
	}

	cout << "Input name of input file: \n";
	cin >> myfile;												// User chooses file to input   -> Name of file of inputs

	string input;
	ifstream inputFile;
	inputFile.open(myfile.c_str());
	if (!inputFile.is_open()) {									// If file name is invalid, error
		cout << "Error: Invalid file. \n";
		return 0;
	}

// Once receives 3 inputs, begins constructing new GenericGraph

	int vertices = 0;											// Number of vertices
	int edges = 0;												// Number if edges
	double weight;
	string vert;												// Vertex value
	vector<string> edgeList;									// List of edges gathered from algorithm

	if (inputFile.is_open()) {									// If file exists, runs file
		getline(inputFile, input);
		ss.clear();
		ss.str("");
		ss << input;
		ss >> vertices >> edges;
		GenericGraph N (vertices, edges);
		GenericGraph K (vertices, edges);
		GenericGraph P (vertices, edges);
		GenericGraph Y (vertices, edges);

		for (int i = 0; i < vertices; ++i) {
			getline(inputFile, input);
			vert = input;
			int result = N.vertex(vert);																				// Vertex put into tree, must output results if error occurs
			result = K.vertex(vert);
			result = P.vertex(vert);
			result = Y.vertex(vert);
			if (result == -1) {
				cout << "Error: Attempt to reassign same name to another vertex.\n";
			}
			if (result == -2) {
				cout << "Error: Attempt to initialize more than |V| vertices.\n";
			}
		}
		// G.print();
		for (int j = 0; j < edges; ++j) {																				// Inputs edges
			getline(inputFile, input);
			ss.clear();
			ss.str("");
			ss << input;
			string name;
			string vert1;
			string vert2;
			ss >> name >> weight >> vert1 >> vert2;
			int result = N.edge (name, weight, vert1, vert2);
			result = K.edge (name, weight, vert1, vert2);
			result = P.edge (name, weight, vert1, vert2);
			result = Y.edge (name, weight, vert1, vert2);
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
	

		if (!N.valid()) {											// Checks if graph is valid
			cout << "Error: Graph produced was not valid.\n";
			return 0;
		}

		N.setDisjoints();											// Sets all disjoint sets
		N.setDebug (debug);
		K.setDisjoints();											// Sets all disjoint sets
		K.setDebug (debug);
		P.setDisjoints();											// Sets all disjoint sets
		P.setDebug (debug);
		Y.setDisjoints();											// Sets all disjoint sets
		Y.setDebug (debug);


			// double weight = G.NUTs (edgeList);
		string newOutputFile;
		for (int i = 0; i < myfile.size() - 4; ++i) {				// Creates new string for output file
			newOutputFile.push_back(myfile.at(i));
		}
		newOutputFile += ".out";

		ofstream outputFile;
		outputFile.open(newOutputFile.c_str());

		if (!outputFile.is_open()) {
			ofstream outputFile (newOutputFile.c_str());
		}
		double weight = 0;
		if (outputFile.is_open()) {
			if (algorithm == "-n" || algorithm == "-a") {
				outputFile << "Algorithm: NUTs \n";
				weight = N.NUTs (edgeList);
				outputFile << "Weight of the tree: " << weight << endl;
				outputFile << "Edges in the tree: ";
				for (int i = 0; i < edgeList.size(); ++i) {
					outputFile << edgeList.at(i) << ' ';
				}
				vector<vector<string>> fragments;
				int frags = N.treeDetails(edgeList, fragments);
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
			if (algorithm == "-k" || algorithm == "-a") {
				outputFile << "Algorithm: Kruskal's \n";
				edgeList.resize(0);
				weight = K.Kruskal (edgeList);
				outputFile << "Weight of the tree: " << weight << endl;
				outputFile << "Edges in the tree: ";
				for (int i = 0; i < edgeList.size(); ++i) {
					outputFile << edgeList.at(i) << ' ';
				}
				vector<vector<string>> fragments;
				int frags = K.treeDetails(edgeList, fragments);
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
			if (first_two == "-p" || algorithm == "-a") {
				outputFile << "Algorithm: Prim's \n";
				edgeList.resize(0);
				weight = P.Prim (rootIndex, edgeList);
				weight = P.PrimPlus (edgeList);
				outputFile << "Weight of the tree: " << weight << endl;
				outputFile << "Edges in the tree: ";
				for (int i = 0; i < edgeList.size(); ++i) {
					outputFile << edgeList.at(i) << ' ';
				}
				vector<vector<string>> fragments;
				int frags = P.treeDetails(edgeList, fragments);
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
			if (algorithm == "-y" || algorithm == "-a") {
				edgeList.resize(0);
				outputFile << "Algorithm: YAMA \n";
				weight = Y.YAMA(edgeList);
				outputFile << "Weight of the tree: " << weight << endl;
				outputFile << "Edges in the tree: ";
				for (int i = 0; i < edgeList.size(); ++i) {
					outputFile << edgeList.at(i) << ' ';
				}
				vector<vector<string>> fragments;
				int frags = Y.treeDetails(edgeList, fragments);
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
		}
	inputFile.close();
	}


	

	return 0;
}
