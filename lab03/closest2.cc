#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
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
    return out << f.num() << '/' << f.den() ;
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

template <typename Comparable>
int closest2key (Comparable key, const vector<Comparable> & A, int & numCompares);

int main(int argc, char *argv[]) {
   string myfile;
   
   if (argc != 2) 
      cout << "Usage: " << argv[0] << " NAME" << endl;
   else 
   myfile = argv[1];
   ifstream inputFile;
   inputFile.open(myfile.c_str());
   
   char type;
   vector<int> integers;
   vector<double> doubles;
   vector<Fraction> fractions;
   
   if (inputFile.is_open()) {
      inputFile >> type;   // size of the array to be allocated
      if (type == 'i' || type == 'I') {
         int number;
         while (inputFile >> number) {
            integers.push_back(number);
            if (inputFile.eof()) break;
         }
      }
      else if (type == 'd' || type == 'D') {
         double number;
         while (inputFile >> number) {
            doubles.push_back(number);
            if (inputFile.eof()) break;
         }
      }
      else if (type == 'f' || type == 'F') {
         int num, den;
         while (inputFile >> num) {
            inputFile >> den;
            if (den == 0) {
               // If denominator is zero, treated as fatal error
               cout << "Fatal Error: Denominator == 0\n";
               return 0;
            }
            Fraction number(num,den);
            fractions.push_back(number);
            if (inputFile.eof()) break;
         }
      }
      else {
         cout << "First input item from file is not of valid value type. Please change and try again\n";
         return 0;
      }
      inputFile.close();
   }
   else {
         cout << "Unable to open file. ";
      do {
         cout << "Input type, followed by each input number of type (type 'end-of-file'): " << endl;
         cin >> type;
         if (type == 'i' || type == 'I' || type == 'd' || type == 'D' || type == 'f' || type == 'F') {
            break;
         }
         cout << "Type invalid. Try again.\n";
      } while (1);  // If size is negative, will be asked to input positive number
      
      if (type == 'i' || type == 'I') {
         int number;
         while (cin >> number) {
            integers.push_back(number);
         }
         cin.clear();
         cin.sync();
         // type = 'i';
         // while (i < integers.size)
      }
      else if (type == 'd' || type == 'D') {
         double number;
         while (cin >> number) {
            doubles.push_back(number);
         }
         cin.clear();
         cin.sync();
         
      }
      else if (type == 'f' || type == 'F') {
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
      }
   }
   

   cout << "Please enter a key (type 'end-of-file' to exit): ";
   if (type == 'f' || type == 'F') {
      int num, den;
      while (cin >> num) {
         cin >> den;
         if (den == 0) {
            cout << "Fatal Error: Denominator == 0\n";
            return 0;
         }
         Fraction fracKey(num,den);
         int numCompares = 0;
         cout << "Solution: " << closest2key(fracKey, fractions, numCompares) << endl;
         cout << "Compares: " << numCompares << endl;
         cout << "Please enter a key (type 'end-of-file' to exit): ";
      }
   }
   else if (type == 'i' || type == 'I') {
      int key;
      while (cin >> key) {
         int numCompares = 0;
         int solution = closest2key(key, integers, numCompares);
         cout << "Solution: " << solution << endl;
         cout << "Compares: " << numCompares << endl;
         cout << "Please enter a key (type 'end-of-file' to exit): ";
      }
   }
   else if (type == 'd' || type == 'D') {
      double key;
      while (cin >> key) {
         int numCompares = 0;
         int solution = closest2key(key, doubles, numCompares);
         cout << "Solution: " << solution << endl;
         cout << "Compares: " << numCompares << endl;
         cout << "Please enter a key (type 'end-of-file' to exit): ";
      }
   }
   cout << endl;
   
}

template <typename Comparable>
int closest2key (Comparable key, const vector<Comparable> & A, int & numCompares) {
   // Find closest match in array A[] to key
   // n is size
   int n = A.size();
   numCompares = 0;
   
   if (n < 1) return -1;
   
   Comparable tempNum(0);
   Comparable compareNum(0);
   
   int index = 0;
   for (auto i = A.begin(); i < A.end(); i++) {
      numCompares += 1;
      if (key == *i) return i - A.begin();
      else if ((i - A.begin()) == 0) {
         // First iteration set stempNum
         tempNum = key - *i;
         // tempNum = abs(tempNum);
         if (tempNum < 0) tempNum *= -1;
         index = i - A.begin();
         cout << "A[" << index << "] = " << A.at(index) << ", ";
         cout << "Key distance = " << tempNum << endl;
      }
      
      compareNum = key - *i;
      // compareNum = abs(compareNum);
      if (compareNum < 0) compareNum *= -1;
      
      if (compareNum < tempNum) {
         index = i - A.begin();
         
                  // cout << "tempNum: " << tempNum << endl;
                  // cout << "compareNum: " << compareNum << endl;

         tempNum = compareNum;
         cout << "A[" << index << "] = " << A.at(index) << ", ";
         cout << "Key distance = " << compareNum << endl;
      }
   }
   return index;
}