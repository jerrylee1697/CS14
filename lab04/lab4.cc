#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>
#include <cmath>
using namespace std;

class Fraction
{
        int gcd(int a, int b) { return b==0 ? a : gcd(b,a%b); }
        int n, d;
    public:
        Fraction(int n, int d = 1) : n(n/gcd(std::abs(n),std::abs(d))), d(d/gcd(std::abs(n),std::abs(d))) {
            if (this->d < 0) {
                this->n *= -1;
                this->d *= -1;
            }
        }
        int num() const { return n; }
        int den() const { return d; }
        Fraction& operator*=(const Fraction& rhs) {
            int new_n = n*rhs.n / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            d = d*rhs.d / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            n = new_n;
            return *this;
        }
        Fraction& operator-=(const Fraction& rhs) {
            int new_n = n*rhs.d - d*rhs.n / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            d = d*rhs.d / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            n = new_n;
            return *this;
        }
};

std::ostream& operator<<(std::ostream& out, const Fraction& f) {
    return out << f.num() << "/" << f.den() ;
}
bool operator==(const Fraction& lhs, const Fraction& rhs) {
    return ((lhs.num() * rhs.den()) == (rhs.num() * lhs.den()));
}
bool operator<(const Fraction& lhs, const Fraction& rhs) {
    return (lhs.num() * rhs.den() < rhs.num() * lhs.den());
}
bool operator>(const Fraction& lhs, const Fraction& rhs) {
    return (lhs.num() * rhs.den() > rhs.num() * lhs.den());
}
Fraction operator*(Fraction lhs, const Fraction& rhs) {
    return lhs *= rhs;
}
Fraction operator-(Fraction lhs, const Fraction& rhs) {
    return lhs -= rhs;
}

template<typename T>
int split_list (list<T> & in_list, list<T> & reject_list, int k);

template<typename T>
void print_list(list<T> & list);

template<typename T>
void mergesort (list<T> & myList, typename list<T>::iterator & L, int blockSize);

template<typename T>
typename list<T>::iterator merge(list<T> & myList, typename list<T>::iterator & L1, int Lblock, typename list<T>::iterator & L2, int Rblock);

template<typename T>
void print_list (list<T> & myList, typename list<T>::iterator P, typename list<T>::iterator P1, typename list<T>::iterator P2, int blockSize);


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
   
   string type;
   stringstream ss;
   list<int> integers;
   list<double> doubles;
   list<Fraction> fractions;
 
   if (inputFile.is_open()) {
      ss.clear();
      ss.str("");
      getline(inputFile,input);   // size of the array to be allocated
      ss << input;
      ss >> type;

      if (type == "i" || type == "I") {
         int number;
         int numbertopush;
         while (inputFile >> number) {                // Takes inputs from file into ss stream into list
            ss.clear();
            ss.str("");
            ss << number;
            ss >> numbertopush;
            integers.push_back(numbertopush);
         }
         cout << "List: ";                            // Outputs List: Integers
         print_list(integers);
         list<int>::iterator intIT = integers.begin();
         mergesort(integers, intIT, integers.size());                             // Sorts List
         cout << "Sorted: ";                          // Outputs Sorted List
         print_list(integers);
      }
      else if (type == "d" || type == "D") {
         double number;
         double numbertopush;
         while (inputFile >> number) {
            ss.clear();
            ss.str("");
            ss << number;
            ss >> numbertopush;
            doubles.push_back(numbertopush);
         }
         cout << "List: ";                            // Outputs List: Doubles
         print_list(doubles);
         list<double>::iterator doubleIT = doubles.begin();
         mergesort(doubles, doubleIT, doubles.size());                              // Sorts List
         cout << "Sorted: ";                          // Outputs Sorted List
         print_list(doubles);
      }
      else if (type == "f" || type == "F") {
         int num, den;
         int numpush, denpush;
         while (inputFile >> num) {
            ss.clear();
            ss.str("");
            inputFile >> den;
            ss << num;
            ss >> numpush;
            ss.clear();
            ss.str("");
            ss << den;
            ss >> denpush;
            if (den == 0) {
               // If denominator is zero, treated as fatal error
               cout << "Fatal Error: Denominator == 0\n";
               return 0;
            }
            Fraction number(numpush,denpush);
            fractions.push_back(number);
         }
         cout << "List: ";                            // Outputs List: Fractions
         print_list(fractions);
         list<Fraction>::iterator fractionIT = fractions.begin();
         mergesort(fractions, fractionIT, fractions.size());                              // Sorts List
         cout << "Sorted: ";                          // Outputs Sorted List
         print_list(fractions);
      }
      else {
         cout << "First input item from file is not of valid value type. Please change and try again\n";
         return 0;
      }
      inputFile.close();
   }
   else {
         cout << "Unable to open file. ";             // Manual inputs if unable to open file
      do {
         cout << "Input type, followed by each input number of type (ctrl-D to end): " << endl;
         cin >> type;
         if (type == "i" || type == "I" || type == "d" || type == "D" || type == "f" || type == "F") {
            break;
         }
         cout << "Type invalid. Try again.\n";
      } while (1);  // If size is negative, will be asked to input positive number
      
      if (type == "i" || type == "I") {
         int number;
         while (cin >> number) {
            integers.push_back(number);
         }
         cin.clear();
         cin.sync();
         cout << "List: ";                            // Outputs List: Integers
         print_list(integers);
         list<int>::iterator intIT = integers.begin();
         mergesort(integers, intIT, integers.size());                             // Sorts List
         cout << "Sorted: ";                          // Outputs Sorted List
         print_list(integers);
      }
      else if (type == "d" || type == "D") {
         double number;
         while (cin >> number) {
            doubles.push_back(number);
         }
         cin.clear();
         cin.sync();
         cout << "List: ";                            // Outputs List: Doubles
         print_list(doubles);
         list<double>::iterator doubleIT = doubles.begin();
         mergesort(doubles, doubleIT, doubles.size());                              // Sorts List
         cout << "Sorted: ";                          // Outputs Sorted List
         print_list(doubles);
      }
      else if (type == "f" || type == "F") {
         int num, den;
         while (cin >> num) {
            cin >> den;
            if (den == 0) {
               cout << "Fatal Error: Denominator == 0\n";
               return 0;
            }
            Fraction number(num,den);
            fractions.push_back(number);
         }
         cout << "List: ";                            // Outputs List: Fractions
         cin.clear();
         cin.sync();
         print_list(fractions);
         cout << "Sorted: ";                  // Outputs Sorted List
         list<Fraction>::iterator fractionIT = fractions.begin();
         mergesort(fractions, fractionIT, fractions.size()); 
         print_list(fractions);
      }
   }
   
   int k = 0;
   int size = 0;
   cout << "Please enter a positive value (k > 0) for the k-th value (type ctrl-D to exit): ";
   while (cin >> k) {
      if (k > 0) break;
      else {
         cout << "Invalid.\n";
         cout << "Please enter a positive value (k > 0) for the k-th value (type ctrl-D to exit): ";
      }
   }
    
   
   if (type == "f" || type == "F") {
      // int num, den;
      // cin >> k;
      list<Fraction> rejects; 
      size = split_list (fractions, rejects, k);
      cout << "Reject List: ";
      print_list(rejects);
      cout << "Size of list: " << size << endl;
      cout << "Original List: ";
      print_list(fractions);
   }
   else if (type == "i" || type == "I") {
      // int key;
      // cin >> k;
      list<int> rejects;
      size = split_list (integers, rejects, k);
      cout << "Reject List: ";
      print_list(rejects);
      cout << "Size of list: " << size << endl;
      cout << "Original List: ";
      print_list(integers);
   }
   else if (type == "d" || type == "D") {
      // cin >> k;
      list<double> rejects;
      size = split_list (doubles, rejects, k);
      cout << "Reject List: ";
      print_list(rejects);
      cout << "Size of List: " << size << endl;
      cout << "Original List: ";
      print_list(doubles);
   }
}

template<typename T>
int split_list (list<T> & in_list, list<T> & reject_list, int k) {
   typename list<T>::iterator it = in_list.begin();
   // cout << "test\n";
   int count = k;
   k -= 1;
   while (count <= in_list.size())  {
      // cout << k << " ";
      advance (it, k);
      reject_list.push_back(*it);
      it = in_list.erase(it);
      count += k;
   }
   // cout << reject_list.size() << "check\n";
   return reject_list.size();
}

template<typename T>
void print_list(list<T> & list) {
   for (T n : list) {
      cout << n << " ";
   }
   cout << endl;
   return;
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