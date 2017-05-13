#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

template <typename Comparable>
class GenericTree {
	public:
		GenericTree(Comparable root_val) {
			root = new TreeNode(root_val);
		}
		int insert (Comparable new_val, Comparable parent_val) {
			TreeNode *key_loc; //= new TreeNode();
			if (contains(new_val, root, key_loc)) return -1;								// If new_val is found, returns -1
			if (!contains(parent_val, root, key_loc)) return -2;							// If parent_val not found, returns -2
			
			TreeNode *newVal = new TreeNode(new_val);										// New value created if parent exists and value doesnt already exist

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

			return 1;																		// If succeeds, returns 1
		}
		void print_Tree () {																// Print_tree calls to print-tree helper with root
			vector<int> nodeLabel; 
			nodeLabel.push_back(1);
			print_Tree(root, nodeLabel);
			return;
		}

		int set_height() {																	// Set_height calls to set-height helper with root
			set_height(root);
			return root->height;
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

	bool contains (const Comparable & key_val, TreeNode *rt, TreeNode * & key_loc) {		// Checks entire tree to see if the value being searched for is contained in tree
		if (rt == NULL) {																	// If not, returns false
			return false;
		}
		if (rt->element == key_val) {														// If true, returns true with the node location store in key_loc
			key_loc = rt;
			return true;
		}
		else if (key_val != rt->element) {
			if (rt->firstChild != NULL) {
				if (contains (key_val, rt->firstChild, key_loc) == false) {
					if  (contains (key_val, rt->nextSibling, key_loc) == true){
						return contains (key_val,rt->nextSibling,key_loc);
					}
				}
				if (contains (key_val, rt->firstChild, key_loc) == true) {
					return contains (key_val, rt->firstChild, key_loc);
				}
			}
			if (rt->nextSibling != NULL) {
				if (contains (key_val, rt->nextSibling, key_loc) == false) {
					rt = rt->nextSibling;
					if (contains (key_val, rt->nextSibling, key_loc) == true) {
						return contains (key_val, rt->nextSibling, key_loc);
					}
				}
				if (contains (key_val, rt->nextSibling, key_loc) == true) {
					return contains (key_val, rt->nextSibling, key_loc);
				}
			}
			return false;
		}
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
				// if (what_happened == 1) cout << "Success\n";								// Test for "what_happened" - if insertion succeeded or failed
				// else if (what_happened == -1) {
				// 	cout << "Failure: new_val found\n"; 
				// }
				// else if (what_happened == -2) {
				// 	cout << "Failure: parent not found\n";
				// }
			}		
			height = t.set_height();														// Sets height of all nodes and returns height of root to "height"	
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
				// if (what_happened == 1) cout << "Success\n";
				// else if (what_happened == -1) {
				// 	cout << "Failure: new_val found\n"; 
				// }
				// else if (what_happened == -2) {
				// 	cout << "Failure: parent not found\n";
				// }
			}
			height = t.set_height();
			t.print_Tree();
		}
		cout << "Height of Tree: " << height << endl;										// Outputs height of entire tree
	}
	return 0;
}