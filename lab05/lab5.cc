#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
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

template <typename T>
int Marjorie(vector<T> & V);                                                  // Sorts using heapsort with default: left = 0, right = Size - 1

template <typename T>                                                             
int Marjorie(vector<T> & V, int left, int right);                             // Sorts with set left and right

template <typename T>
int buildHeap (vector<T> & V, int left, int right);                           // Helper function to build heap

template <typename T> 
void printHeap (vector<T> & V, int left, int right);                          // Prints heap in given range (right - left)

template<typename T>
void deleteMax (vector<T> & V, int & left, int & right, T & maxItem);         // "Deletes" maxHeap

template <typename T>
void print_list(vector<T> & V, int left, int right);                          // Prints list with brackets


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
   int left = 0;                                                              // Leftmost index in vector
   int right = 0;                                                             // Rightmost index of vector
   int valid = 0;                                                             // Declares validity of heapsort (if it fails or not)
   stringstream ss;
   vector<int> integers;
   vector<double> doubles;
   vector<Fraction> fractions;

   if (inputFile.is_open()) {
      getline(inputFile,input); 
      ss.clear();
      ss.str("");                                                             // size of the array to be allocated
      ss << input;
      ss >> type;                                                             // SS stream takes type
      if (ss >> left) {                                                       // SS stream inputs left and right if valid
         ss >> right;
      }
      else {                                                                  // Otherwise, makes default sizing (left = 0, right = size - 1)
         left = 0;
         right = -1;                
      }
      getline(inputFile, input);                                              // Getline second line into stream for parsing

      ss.clear();
      ss.str("");
      ss << input;
      
      if (type == "i" || type == "I") {
         int numbertopush;
         while (ss >> numbertopush) {                                        // Takes inputs from file into ss stream into list
            integers.push_back(numbertopush);
         }
         cout << "Integers List: ";                                           // Outputs List: Integers
         if (right == -1) {                                                   // Checks if Right was declared
            right = integers.size() - 1;
         }
         print_list(integers, left, right);
         valid = Marjorie (integers, left, right);                            // heapSort list
         if (valid == 1) {                                                    // If valid, outputs
            cout << "Sorted: ";                                               // Outputs Sorted List
            print_list(integers, left, right);                                // Outputs Sorted List with brackets
         }
         else {
            cout << "Error. Failed to carry out sort.\n";
            cout << "Possible errors:\n1. Left index is greater than right index .\n2. Right index is greater than that available in vector.\n";
            return 0;
         }
      }
      else if (type == "d" || type == "D") {
         double numbertopush;
         while (ss >> numbertopush) {
            doubles.push_back(numbertopush);
         }
         cout << "Doubles List: ";                                            // Outputs List: Integers
         if (right == -1) {                                                   // Checks if Right was declared
            right = doubles.size() - 1;
         }
         print_list(doubles, left, right);
         valid = Marjorie (doubles, left, right);                             // heapSort list
         if (valid == 1) {                                                    // If valid, outputs
            cout << "Sorted: ";                                               // Outputs Sorted List
            print_list(doubles, left, right);                                 // Outputs Sorted List with brackets in overloaded function
         }
         else {
            cout << "Error. Failed to carry out sort.\n";
            cout << "Possible errors:\n1. Left index is greater than right index .\n2. Right index is greater than that available in vector.\n";
            return 0;
         }
      }
      else if (type == "f" || type == "F") {
         int num, den;
         int numpush, denpush;
         while (ss >> numpush) {
            ss >> denpush;
            if (den == 0) {
               cout << "Fatal Error: Denominator == 0\n";                     // If denominator is zero, treated as fatal error
               return 0;
            }
            Fraction number(numpush,denpush);
            fractions.push_back(number);
         }
         cout << "Fractions List: ";                                          // Outputs List: Integers
         if (right == -1) {                                                   // Checks if Right was declared
            right = fractions.size() - 1;
         }
         print_list(fractions, left, right);
         valid = Marjorie (fractions, left, right);                           // heapSort list
         if (valid == 1) {                                                    // If valid, outputs
            cout << "Sorted: ";                                               // Outputs Sorted List
            print_list(fractions, left, right);                               // Outputs Sorted List with brackets in overloaded function
         }
         else {
            cout << "Error. Failed to carry out sort.\n";
            cout << "Possible errors:\n1. Left index is greater than right index .\n2. Right index is greater than that available in vector.\n";
            return 0;
         }
      }
      else {
         cout << "First input item from file is not of valid value type. Please change and try again\n";
         return 0;
      }
      inputFile.close();
   }
   else {
         cout << "Unable to open file. ";                                     // Manual inputs if unable to open file
      do {
         cout << "Input type, followed by each input number of type (ctrl-D to end): " << endl;
         getline(cin, input);
         ss.clear();
         ss.str("");
         ss << input;
         ss >> type;
         if (type == "i" || type == "I" || type == "d" || type == "D" || type == "f" || type == "F") {
            if (ss >> left) {
               ss >> right;
            }
            else {
               left = 0;
               right = -1;
            }
            break;
         }
         cout << "Type invalid. Try again.\n";
      } while (1);                                                            // If size is negative, will be asked to input positive number
      
      if (type == "i" || type == "I") {
         int number;
         while (cin >> number) {
            integers.push_back(number);
         }
         cin.clear();
         cin.sync();
         cout << "Integers List: ";  
         if (right == -1) { 
            left = 0;                                        
            right = integers.size() - 1;
         }

         print_list(integers, left, right);                                   // Outputs List: Integers
         valid = Marjorie (integers, left, right);                            // heapSort list

         if (valid == 1) {                                                    // If valid, outputs
            cout << "Sorted: ";                                               // Outputs Sorted List
            print_list(integers, left, right);                                // Outputs Sorted List with brackets in overloaded function
         }
         else {
            cout << "Error. Failed to carry out sort.\n";
            cout << "Possible errors:\n1. Left index is greater than right index .\n2. Right index is greater than that available in vector.\n";
            return 0;
         }
      }
      else if (type == "d" || type == "D") {
         double number;
         while (cin >> number) {
            doubles.push_back(number);
         }
         cin.clear();
         cin.sync();
         cout << "Doubles List: "; 
         if (right == -1) { 
            left = 0;                                        
            right = doubles.size() - 1;
         }                    

         print_list(doubles, left, right);
         valid = Marjorie (doubles, left, right);                             // heapSort list

         if (valid == 1) {                                                    // If valid, outputs
            cout << "Sorted: ";                                               // Outputs Sorted List
            print_list(doubles, left, right);                                 // Outputs Sorted List with brackets in overloaded function
         }
         else {
            cout << "Error. Failed to carry out sort.\n";
            cout << "Possible errors:\n1. Left index is greater than right index .\n2. Right index is greater than that available in vector.\n";
            return 0;
         }
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
         cin.clear();
         cin.sync();
         cout << "Fractions List: ";    
         if (right == -1) { 
            left = 0;                                        
            right = fractions.size() - 1;
         }        

         print_list(fractions, left, right);                                  // Outputs List: integers
         valid = Marjorie (fractions, left, right);                           // heapSort list

         if (valid == 1) {                                                    // If valid, outputs
            cout << "Sorted: ";                                               // Outputs Sorted List
            print_list(fractions, left, right);                               // Outputs Sorted List with brackets in overloaded function
         }
         else {
            cout << "Error. Failed to carry out sort.\n";
            cout << "Possible errors:\n1. Left index is greater than right index .\n2. Right index is greater than that available in vector.\n";
            return 0;
         }
      }
   }
   return 0;
}

template <typename T>
void print_list(vector<T> & V, int left, int right) {
   // Left = leftmost index
   // Right = rightmost index
   int counter = 0;

   for (T n : V) {
      if (left == counter) {
         cout << "[ ";
      }
      cout << n << ' ';
      if (right == counter) {
         cout << "] ";
      }
      ++counter;
   }
   cout << endl;
}

template <typename T>
int Marjorie(vector<T> & V);

template <typename T>
int Marjorie(vector<T> & V, int left, int right) {
   int rightIndex = right;

   while (right >  left) {
      buildHeap(V, left, right);
      printHeap(V, left, rightIndex);
      T maxItem = V.at(left);
      deleteMax(V, left, right, maxItem);
      --right;
   }

   return 1;
}

template <typename T>
int buildHeap (vector<T> & V, int left, int right) {
   // Builds maxHeap
   if (left > right) return -1;
   if (right == V.size()) return -1;
   
   int range = right - left;                                                  // Range to be sorted
   int i = right;
   int j = left - 1;
   int parent = (right + j) / 2;                                              // Parent = ((i - 1) / 2) while i > 0

   for (; parent >= left && (2 * parent - j) <= right;--parent) {
      int lastChecked = parent;                                               // Saves root-parent's index
      while (parent <= (right + j) / 2 && parent >= left) {                   // Right / 2 is last parent to have children
         int Lchild = 2 * parent - j;                                         // Lchild = 2 * parent + 1
         int Rchild = 2 * parent - j + 1;                                     // Rchild = 2 * parent + 2
         if (Rchild > right) {
            if (V.at(Lchild) > V.at(parent)) {
               T temp = move(V.at(Lchild));                                   // Swap if Left child greater / only child under parent
               V.at(Lchild) = move(V.at(parent));  
               V.at(parent) = move(temp);
               break;
            }
            else {
               break;
            }
         }
         if (V.at(Lchild) > V.at(parent) || V.at(Rchild) > V.at(parent)) {
            if (V.at(Lchild) > V.at(Rchild) && (Rchild <= right)) {
               T temp = move(V.at(Lchild));                                   // Swap if Left child greater
               V.at(Lchild) = move(V.at(parent));  
               V.at(parent) = move(temp);    
               parent = Lchild;                                               // New parent to check branch
            }
            else if (V.at(Rchild) > V.at(Lchild) && (Rchild <= right)) {
               T temp = move(V.at(Rchild));                                   // Swap if Right child greater
               V.at(Rchild) = move(V.at(parent));
               V.at(parent) =  move(temp);
               parent = Rchild;                                               // New parent to check branch
            }
            else if (V.at(Lchild) > V.at(parent)) {
               T temp = move(V.at(Lchild));                                   // Swap if Left child greater / only child under parent
               V.at(Lchild) = move(V.at(parent));  
               V.at(parent) = move(temp);
               break;
            }
            else {
               break;
            }
         }
         else {
            break;
         }
      }
      parent = lastChecked;                                                   // Returns check place to last main parent checked, then decrements  
   }
   return 1;                                                                  // Positive one = successfully built heap 
}

template <typename T> 
void printHeap (vector<T> & V, int left, int right) {
   int size = right - left + 1;
   int levels = log2(size);
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
   V.at(left) = move(V.at(right));
   V.at(right) = move(maxItem);
   return;
}

// ............................................________ 

// ....................................,.-‘”...................``~., 

// .............................,.-”...................................“-.
// , 
// .........................,/...............................................”:,

// .....................,?......................................................\, 

// .................../...........................................................,} 

// ................./......................................................,:`^`..} 

// .............../...................................................,:”........./ 

// ..............?.....__.........................................:`.........../ 

// ............./__.(.....“~-,_..............................,:`........../ 

// .........../(_....”~,_........“~,_....................,:`........_/ 

// ..........{.._$;_......”=,_.......“-,_.......,.-~-,},.~”;/....} 

// ...........((.....*~_.......”=-._......“;,,./`..../”............../ 

// ...,,,___.\`~,......“~.,....................`.....}............../ 

// ............(....`=-,,.......`........................(......;_,,-” 

// ............/.`~,......`-...............................\....../\ 

// .............\`~.*-,.....................................|,./.....\,__ 

// ,,_..........}.>-._\...................................|..............`=~-, 

// .....`=~-,_\_......`\,.................................\ 

// ...................`=~-,,.\,...............................\ 

// ................................`:,,...........................`\..............__ 

// .....................................`=-,...................,%`>--==`` 

// ........................................_\..........._,-%.......`\ 

// ...................................,
