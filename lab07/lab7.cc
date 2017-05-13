#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <utility>
using namespace std;

unsigned int hash_key (const double & key, int tableSize);

unsigned int hash_key (const string & key, int tableSize);

template <typename Comparable>
class GenericTree {
	public:
		GenericTree(Comparable root_val) {
			root = new TreeNode(root_val);

			// hashTable.reserve(tableSize);
			hashTable.resize(tableSize);
			int key = hash_key (root_val, tableSize);
			hashTable.at(key) = new HashEntry (root_val, root, ACTIVE);
		}
		int insert (Comparable new_val, Comparable parent_val) {
			TreeNode *key_loc;
			if (contains(new_val, root, key_loc)) return -1;								// If new_val is found, returns -1
			if (!contains(parent_val, root, key_loc)) return -2;							// If parent_val not found, returns -2
			
			TreeNode *newVal = new TreeNode(new_val);										// New value created if parent exists and value doesnt already exist
			
			int key = hash_key (new_val, tableSize);										// Creates hash key

			if (key >= tableSize / 2) {
				rehash();
			}

			if (key_loc->firstChild == NULL) {												// Reiterates through tree to find where to place new node
				key_loc->firstChild = newVal;
			}
			else {
				key_loc = key_loc->firstChild;
				while (key_loc->nextSibling != NULL) {
					key_loc = key_loc->nextSibling;
				}
				key_loc->nextSibling = newVal;
			}
			int i = 0;
			int quad = (key + i * i) % tableSize;											// Quadratic Probing
			while (1) {
				if (hashTable.at(quad) != NULL) {
					i += 1;
					quad = (key + i * i) % tableSize;
				}
				else {
					hashTable.at(quad) = new HashEntry (new_val, newVal, ACTIVE);			// Inserts into hash table
					break;
				}
			}
			return 1;																		// If succeeds, returns 1
		}

		void print_Tree () {																// Print_tree calls to print-tree helper with root
			vector<int> nodeLabel; 
			nodeLabel.push_back(1);
			print_Tree(root, nodeLabel);
			return;
		}

		int set_height () {																	// Set_height calls to set-height helper with root
			set_height(root);
			return root->height;
		}

		void dump_hashtable () {
			HashEntry *it;
			TreeNode *it1;
			for (int i = 0; i < hashTable.size(); ++i) {
				if (hashTable.at(i) != NULL) {
					if (hashTable.at(i)->info == ACTIVE) {
						cout << "Row: " << i << ' ';
						cout << "ACTIVE ";
						cout << '\"' << hashTable.at(i)->key_val << "\" ";
						cout << "hash: " << hash_key(hashTable.at(i)->key_val, tableSize) << ' ';
						cout << "->";
						it1 = hashTable.at(i)->key_loc;
						cout << '\"' << it1->element << '\"' << endl;
					}
				}
			}
			return;
		}

		void hang_left () {
			hang_left (root);
			return;
		}

	private:

		struct TreeNode {																	// Struct creates nodes of type TreeNode
			Comparable element;
			TreeNode *firstChild;															// Pointer to first child
			TreeNode *nextSibling;															// Pointer to next child
			int height; 																	// added because you need it later
			TreeNode () {																	// Constructor for default 
				element;
				firstChild = NULL;
				nextSibling = NULL;
				height = -1;
			}
			TreeNode (Comparable & root_val) {												// Constructor with value
				element = root_val;
				firstChild = NULL;
				nextSibling = NULL;
				height = -1;																// Height not yet set
			}
		};

		TreeNode *root;	

		enum EntryType {ACTIVE, EMPTY, DELETED };

		struct HashEntry {
			Comparable key_val;
			TreeNode *key_loc;
			EntryType info;
			HashEntry( const Comparable & e = Comparable ( ), TreeNode * const n = nullptr, EntryType i = EMPTY) 
				: key_val(e), key_loc (n), info(i) { }
		};

		int tableSize = 134456;
		vector<HashEntry*> hashTable;

		priority_queue <pair <int, TreeNode*>> childQ;

	void rehash () {
		unsigned new_size = hashTable.size();
		new_size = (isPrime(2 * new_size));
		hashTable.resize(new_size);
		tableSize = new_size;
	}

	unsigned isPrime (int num) {
		while (num % 2 == 0 || num % 3 == 0) {
			num += 1;
		}
		return num;
	}

	bool contains (const Comparable & key_val, TreeNode *rt, TreeNode * & key_loc) {		// New Contains function using hashmap
		if (rt == NULL) return false;														// If root is empty, false
		if (rt->element == key_val) {														// If root element is element, true
			key_loc = rt;
			return true;
		}
		int key = hash_key (key_val, tableSize);											// Finds hash-key of key_val
		if (hashTable.at(key) != NULL) {													// If location in table is not null, valid
			if (hashTable.at(key)->key_val != key_val) {									// Checks if value is same
				int i = 0;
				int quad = (key + i * i) % tableSize;										// Quadratic extraction
				while (1) {
					i += 1;
					quad = (key + i * i) % tableSize;
					if (hashTable.at(quad)->key_val == key_val) {
						HashEntry *it = hashTable.at(key);												// Pointer of hashTable at key index is passed to "it"
						key_loc = it->key_loc;															// Pointer from implicit passed to key_loc
						return true;
					}
					if (hashTable.at(quad) == NULL) return false;
				}
			}
			else {
				HashEntry *it = hashTable.at(key);												// Pointer of hashTable at key index is passed to "it"
				key_loc = it->key_loc;															// Pointer from implicit passed to key_loc
				return true;
			}
		}
		return false;
	}

	void set_height(TreeNode *rt) {															// Sets height of entire tree recursively using post-order
		if (rt == NULL) return;																// Postorder: starts from bottom of tree and works way up
		TreeNode *it;
		if (rt->firstChild == NULL) {														// Sets height to zero at every leaf node
			rt->height = 0;
		}
		if (rt->firstChild != NULL) {														// Checks if has first-child
			set_height(rt->firstChild);														// Calls recursively
			int h = 0;
			it = rt->firstChild;
			do {																			// Do-while loop allows to be checked atleast once
				if (it->height >= h) {
					h = it->height;
				}
				it = it->nextSibling;
			} while (it != NULL);
			rt->height = h += 1;
		}
		if (rt->nextSibling != NULL) {
			set_height(rt->nextSibling);
		}
		return;
	}

	void print_Tree (const TreeNode *rt, vector<int> & nodeLabel) {							// Prints entire tree recursively
		if (rt == NULL) return;
		for (int i = 0; i < nodeLabel.size(); ++i) {										// Gives position in branch
			cout << nodeLabel.at(i) << ".";
		}
		cout << "[" << rt->height << "]: ";
		cout << rt->element << endl;
		if (rt->firstChild != NULL) {
			nodeLabel.push_back(1);															// Adds one level per new child
			print_Tree (rt->firstChild, nodeLabel);
			nodeLabel.pop_back();															// Pops back when leaving it
		}
		if (rt->nextSibling != NULL) {
			nodeLabel.at(nodeLabel.size() - 1) += 1;										// Adds to number of sibling
			print_Tree (rt->nextSibling, nodeLabel);
			nodeLabel.at(nodeLabel.size() - 1) -= 1;
		}
	}

	void hang_left (TreeNode *rt) {
		if (rt == NULL) return;																// Postorder: starts from bottom of tree and works way up

		if (rt->firstChild != NULL) {
			TreeNode *c = rt->firstChild;													// 'c' is pointer to rt->firstChild
			TreeNode * it = c->nextSibling;													// Pointer it points to next sibling of 'c'
			while (c != NULL) {																// While not null, will continue to push next nodes onto stack
				childQ.push(make_pair(c->height, c));
				c = c->nextSibling;
			}
			int count = 0;
			while (!childQ.empty()) {
				pair<int, TreeNode*> p = childQ.top();
				if (count == 0) {
					rt->firstChild = p.second;
					it = p.second;
					++count;
				}
				else {
					it->nextSibling = p.second;
					it = p.second;
				}
				childQ.pop();
			}
			it->nextSibling = NULL;
		}
		if (rt->firstChild != NULL) {
			hang_left (rt->firstChild);
		}

		if (rt->nextSibling != NULL) {
			hang_left (rt->nextSibling);
		}
		return;
	}
};


int main(int argc, char *argv[]) {
	string myfile;
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " NAME" << endl;
	}
	else 
   	myfile = argv[1];
	ifstream inputFile;
	inputFile.open(myfile.c_str());		
	char valType;																			// Type of value to be stored 
	int what_happened = 0;	
	int height = 0;																			// Results of insert: Success or Failure
	if (inputFile.is_open()) {
		inputFile >> valType;																// Type chosen
		if (valType == 'd' || valType == 'D') {
			double root = 0.0;
			double input1 = 0.0;															// Value of new node
			double input2 = 0.0;															// Value of parent 
			inputFile >> root;																// First number after type is root
			GenericTree<double> t(root);	

			while (inputFile >> input1) {
				inputFile >> input2;
				what_happened = t.insert(input1, input2);
			}		
			height = t.set_height();														// Sets height of all nodes and returns height of root to "height"	
			cout << "Original Tree: \n";
			t.print_Tree();	
			cout << "\nHashtable: \n";
			t.dump_hashtable();	
			t.hang_left();
			cout << "\nTree reordered by height: \n";
			t.print_Tree();		
		}
		else if (valType == 's' || valType == 'S') {
			string root;
			string input1;
			string input2;
			inputFile >> root;
			GenericTree<string> t(root);
			while (inputFile >> input1) {
				inputFile >> input2;
				what_happened = t.insert(input1, input2);
			}
			height = t.set_height();
			cout << "Original Tree: \n";
			t.print_Tree();
			cout << "\nHashtable: \n";
			t.dump_hashtable();
			t.hang_left();
			cout << "\nTree reordered by height: \n";
			t.print_Tree();
		}
		cout << "Height of Tree: " << height << endl;										// Outputs height of entire tree
	}
	return 0;
}

unsigned int hash_key (const string & key, int tableSize) {									// Hash for strings
	unsigned int hashVal = 0;

	for (char ch : key) {
		hashVal = 37 * hashVal + ch;
	}

	hashVal = hashVal % tableSize;
	return hashVal;
}

unsigned int hash_key (const double & key, int tableSize) {									// Hash for doubles
	// unsigned int hashVal = 0;

	int x = floor(key);
	double y = key - x;

	int z = floor(y * 7919);

	return ((28411 + (x + z) * 8121) % 134456);
}

