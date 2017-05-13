#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <list>
#include <cmath>
using namespace std;

template <typename T>
int Marjorie(vector<T> & V);

template <typename T>
int Marjorie(vector<T> & V, int left, int right);

template <typename T>
int buildHeap (vector<T> & V, int left, int right);

template <typename T> 
void printHeap (vector<T> & V, int left, int right);

template<typename T>
void deleteMax (vector<T> & V, int & left, int & right, T & maxItem);

//For testing only
template<typename T>
void print(vector<T> & V);

int main() {
	// vector<int> V = {2,5,7,3,6,9,4};
	// vector<int> V = {803,392,165,122,623,865,422,674,611,57,389,223,564,182,477,544,394,642,780,877,847,21,887,766,446,668,107,96,170,946,188};
	// vector<int> V = {9,8,7,6,5,4,3,2,1};
		vector<int> V = {1,2,3,4,5,6,7,8,9};

	print(V);
	printHeap(V, 0, V.size() - 1);
	int j = Marjorie(V, 0, V.size() - 1);
	cout << "sorted: \n"; 
	printHeap(V, 0, V.size() - 1);

	
	
	// int sorted = Marjorie(V);
	// cout << sorted << endl;
	if (j == 1) {
		print(V);
	}
	else {
		cout << "Not Sorted\n";
	}
	return 0;
}

template <typename T>
int Marjorie(vector<T> & V);

template <typename T>
int Marjorie(vector<T> & V, int left, int right) {
	while (right >  left) {
		// cout << "right: " << right << endl;
		buildHeap(V, left, right);
		T maxItem = V.at(left);
		// printHeap(V, 0, V.size() - 1);
		// print(V);
		deleteMax(V, left, right, maxItem);
		--right;
		printHeap(V, left, V.size() - 1);
		// printHeap(V, 0, V.size() - 1);
		// printHeap(V, 0, V.size() - 1);	
		// print(V);
	}
	return 1;
}

template <typename T>
int buildHeap (vector<T> & V, int left, int right) {
	// Builds maxHeap
	if (left > right) return -1;
	if (right == V.size()) return -1;
	
	int range = right - left;						// Range to be sorted
	int i = right;
	int j = left - 1;
	int parent = (right + j) / 2;					// Parent = ((i - 1) / 2) while i > 0
	// cout << "parent: " << parent << " left: " << left << " right: " << right << " range: " << range << endl;
	// cout << "loopin\n";
	for (; parent >= left && (2 * parent - j) <= right;--parent) {
		int lastChecked = parent;					// Saves root-parent's index
		// cout << "parent: " << parent << " range: " << range << endl;
		while (parent <= (right + j) / 2 && parent >= left) {				// Right / 2 is last parent to have children
			// cout << "parent: " << parent << endl;
			// print(V);
			int Lchild = 2 * parent - j;			// Lchild = 2 * parent + 1
			int Rchild = 2 * parent - j + 1;			// Rchild = 2 * parent + 2
			if (Rchild > right) {
				if (V.at(Lchild) < V.at(parent)) {
					T temp = move(V.at(Lchild));	// Swap if Left child greater / only child under parent
					V.at(Lchild) = move(V.at(parent));	
					V.at(parent) = move(temp);
					// cout << "Swap3\n";
					break;
				}
				else {
					// cout << "break0.5\n";
					break;
				}
			}
			if (V.at(Lchild) < V.at(parent) || V.at(Rchild) < V.at(parent)) {
				if (V.at(Lchild) < V.at(Rchild) && (Rchild <= right)) {
					T temp = move(V.at(Lchild));	// Swap if Left child greater
					V.at(Lchild) = move(V.at(parent));	
					V.at(parent) = move(temp);		
					// cout << "Swap1\n";
					parent = Lchild;				// New parent to check branch
				}
				else if (V.at(Rchild) < V.at(Lchild) && (Rchild <= right)) {
					T temp = move(V.at(Rchild));			// Swap if Right child greater
					V.at(Rchild) = move(V.at(parent));
					V.at(parent) =  move(temp);
					// cout << "Swap2\n";	
					parent = Rchild;				// New parent to check branch
				}
				else if (V.at(Lchild) < V.at(parent)) {
					T temp = move(V.at(Lchild));	// Swap if Left child greater / only child under parent
					V.at(Lchild) = move(V.at(parent));	
					V.at(parent) = move(temp);
					// parent = Lchild;
					// cout << "Swap3\n";
					break;
				}
				else {
					// cout << "break1\n";
					break;
				}
			}
			else {
				// parent = parent - 1;
				// cout << "break2\n";
				break;
			}
		}
		parent = lastChecked; 						// Returns check place to last main parent checked, then decrements	
	}
	return 1;										// Positive one = successfully built heap	
}

template <typename T> 
void printHeap (vector<T> & V, int left, int right) {
	// "Size" = right - left
	int size = right - left + 1;
	int levels = log2(size);
	// cout << "right" << right << " left " << left << "levels " << levels << endl;
	for (int i = 0; i <= levels; ++i) {
		cout << left << ": ";
		for (int j = 0; j < pow(2,i) && left <= right; ++j) {
			cout << V.at(left) << ' ';
			left += 1;
		}
		cout << endl;
	}
	return;
}

template<typename T>
void deleteMax (vector<T> & V, int & left, int & right, T & maxItem) {
	// maxItem = move(V.at(left));
	// cout << "Right to del: " << right << endl;
	V.at(left) = move(V.at(right));
	V.at(right) = move(maxItem);
	// cout << "New: " << V.at(left) << endl;
	return;
}

template <typename T>
void print(vector<T> & V) {
	for (int n : V) {
		cout << n << ' ';
	}
	// for (int i = 0; i < V.size() - 1; ++i) {
	// 	cout << V.at(i) << ' ';
	// }
	cout << endl;
	return;
}