#include <iostream>
using namespace std;
#include <vector>

int main() {
   double number;
   vector<double> doubles;
   while (cin >> number) {
      doubles.push_back(number);
   }
   // cin.clear();
   // cin.sync();
   cin.ignore(1000);
   cin.clear();
   cin.sync();
   while(1) {
      cin >> number;
      cout << number << endl;
   }
   
   return 0;
}
