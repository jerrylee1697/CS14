#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>
using namespace std;

template<typename T>
void mergesort (list<T> & myList, typename list<T>::iterator & L, int blockSize);

template<typename T>
typename list<T>::iterator merge(list<T> & myList, typename list<T>::iterator & L1, int Lblock, typename list<T>::iterator & L2, int Rblock);

template<typename T>
void print_list (list<T> & myList, typename list<T>::iterator P, typename list<T>::iterator P1, typename list<T>::iterator P2, int blockSize);

int main() {
    
    list<int>list1 = {4,2,3,11,13,-1,99,0};
    
    typename list<int>::iterator it = list1.begin();

    mergesort(list1, it, list1.size());

    return 0;
}

template<typename T>
void mergesort (list<T> & myList, typename list<T>::iterator & L, int blockSize){
    if (blockSize < 2) return;
    int Lblock = blockSize / 2;                 // Size of each block
    int Rblock = blockSize - Lblock;            // Size of each block
    
    typename list<T>::iterator s1 = L;          // Iterator of first half points to beginning
    advance(L, Lblock);
    typename list<T>::iterator s2 = L;          // Moves iterator to start of second half
    // advance(s2, Lblock);
    
    mergesort(myList, s1, Lblock);
    mergesort(myList, s2, Rblock);
    
    L = merge(myList, s1, Lblock, s2, Rblock);
    
    return;
}

template<typename T>
typename list<T>::iterator merge(list<T> & myList, typename list<T>::iterator & L1, int Lblock, typename list<T>::iterator & L2, int Rblock) {
    typename list<T>::iterator L = L1;
    int totalSize = Lblock + Rblock;
    for (int i = 0, j = 0; i < Lblock || j < Rblock;){
        print_list(myList, L, L1, L2, totalSize);
        // cout << "L: " << *L << " L1: " << *L1 << " L2: " << *L2 << endl;
        if (*L1 < *L2 || j == Rblock) {
            advance(L1,1);
            ++i;
        } 
        // if (L1 == L2) {
        //     // advance(L1, 1);
        //     advance(L2, 1);
        //     ++j;
        // }
        else {
            T number = *L2;                                     // Number to be inserted
            if (L == L1) {
                L = myList.insert(L1, number);                      // If inserts at front, relocates L
                L2 = myList.erase(L2);
            }
            else {
                myList.insert(L1, number);                  // Insert saved value into new node directly ahead of L1
                if (L1 == L2) {
                    L2 = myList.erase(L2);
                    L1 = L2;
                }
                else {
                    L2 = myList.erase(L2);
                }
            }
            ++j;
        }
        // cout << "L: " << *L << " L1: " << *L1 << " L2: " << *L2 << endl;
    }
    print_list(myList, L, L1, L2, totalSize);
    return L;
}

template<typename T>
void print_list (list<T> & myList, typename list<T>::iterator P, typename list<T>::iterator P1, typename list<T>::iterator P2, int blockSize){
    // P  - ( L )  - iterator pointing to start of the curren output block
    // P1 - ( L1 ) - iterator pointing to start of the current left sub-block
    // P2 - ( L2 ) - iterator pointing to start of the current right sub-block
    int counter = 0;
    typename list<T>::iterator itr = myList.begin();
    while (itr != P) {                                                                  // Adjust blockSize for previously sorted numbers
        blockSize += 1;
        advance(itr, 1);
    }
    itr = myList.begin();                                                               // Restart iteration
    while (itr != myList.end()) {
        if (itr == P) {
            cout << "[ ";
        }
        if (itr == P1) {
            cout << ") ";
        }
        if (itr == P2) {
            cout << ") ";
        }
        if (counter == blockSize) {
            cout << "] ";
        }
        
        cout << *itr << ' ';
        ++counter;
        advance(itr, 1);
        
        if (counter == blockSize && itr == myList.end() && P2 == myList.end()) {        // Checks for P2 at end of list
            cout << ") ";
        }
        if (counter == blockSize && itr == myList.end() && P1 == myList.end()) {        // Checks for P1 at end of list
            cout << ") ";
        }
        if (counter == blockSize && itr == myList.end()) {                              // Checks for end of List
            cout << "] ";
        }
        
    }
    cout << endl;
    return;
}