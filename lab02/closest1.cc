#include <iostream>
using namespace std;
#include <fstream>
#include <algorithm>
#include <cmath>

int closest2key (double key, double A[], int n, int & numCompares);

int closestBinary (double key, double A[], int n, int & numCompares);

int giantStepper (double key, double A[], int n, int & numCompares);


int main(int argc, char *argv[]) {
   string myfile;
   
   if (argc != 2) 
      cout << "Usage: " << argv[0] << " NAME" << endl;
   else 
   myfile = argv[1];
   ifstream inputFile;
   inputFile.open(myfile.c_str());
   
   int size = 0;           // Size of array, not yet declared
   double *array;          // Array dynamically allocated
   double number = 0;      // Value to go into array
   
   if (inputFile.is_open()) {
      inputFile >> size;   // size of the array to be allocated
      
      // cout << "Initial size: " << size << endl;  // Check for size
      
      int i = 0;
      array = new double [size];
      while (inputFile >> number || i < size) {
         array[i] = number;
         ++i;
         if (inputFile.eof()){
            size = i;
            break;
         }
      }
      inputFile.close();
   }
   else {
         cout << "Unable to open file. ";
      do {
         cout << "Input number of inputs (must be positive), followed by each input number: " << endl;
         cin >> size;
      } while (size < 0);  // If size is negative, will be asked to input positive number
      
      array = new double [size];
      for (int i = 0; i < size; ++i) {
         cin >> number;
         array[i] = number;
      }
   }
   
   //  Test for values inside array
   // for (int i = 0; i < size; ++i) cout << array[i] << endl;
   // cout << size << endl;

   // Sort array
   double *sortedArray = new double [size];   // New Array for Sorted
   for (int i = 0; i < size; ++i) {
      sortedArray[i] = array[i];
   }
   sort(sortedArray, sortedArray + size);
   
   // Check for sorted array
   // for (int i = 0; i < size; ++i)
   // {
   //    cout << sortedArray[i] << endl;
   // }
   
   double key = 0.0;             // User inserts key to be searched closest to
   int numCompares = 0;    // initialize number of comparisons to zero
   
   for (int i = 0; i != -1;) {
      cout << "Enter new key value (exit with cntrl-Z/cntrl-D when done): ";
      cin >> key;
      
      if (cin.fail()) return 0;
      
      int index = closest2key(key, array, size, numCompares);
      cout << "closest2key " << index << " " << array[index] << " " << numCompares << endl;
      
      int index2  = closestBinary(key, sortedArray, size, numCompares);
      cout << "closestBinary " << index2 << " " << sortedArray[index2] << " " << numCompares << endl;
   
      int index3 = giantStepper(key, sortedArray, size, numCompares);
      cout << "giantStepper " << index3 << " " << sortedArray[index3] << " " << numCompares << endl;
   }
   return 0;
}

int closest2key (double key, double A[], int n, int & numCompares){
   // Find closest match in array A[] to key
   // n is size
   numCompares = 0;        // Initialized to zero at beginning of function
                           // incremented each time key value compared to any element in array
   if (n < 1) return -1;
   double tempNum = 0;     // Use number to compare
   double compareNum = 0;
   int j = 0;              // Index of closest
   
   for (int i = 0; i < n; ++i) {
      numCompares += 1;    // tracks number of comparisons made
      if (key == A[i]) return i;    // Returns index if found exactly
      else if (i == 0) {
         // First iteration sets tempNum
         tempNum = key - A[i];
         tempNum = abs(tempNum);
         j = i;
      }
      // Normal iterations after first iteration
      compareNum = key - A[i];
      compareNum = abs(compareNum);
      
      if (compareNum < tempNum) {
         j = i;
         tempNum = compareNum;
      }
   }
   return j;
}

int closestBinary (double key, double A[], int n, int & numCompares) {
   if (n < 1) return -1;
   
   int low = 0, high = n - 1;
   int mid = 0;
   numCompares = 0;
   
   while (low <= high) {
      
      mid = (low + high) / 2;

      if (A[mid] < key) {
         low = mid + 1;
      }
      else if (A[mid] > key) {
         if (mid == 0) break;
         high = mid - 1;
         mid -= 1;
      }
      else {
          break;
      }
      numCompares += 1;
   }
   // mid -= 1;
   double compare1 = A[mid + 1] - key;
   double compare2 = A[mid] - key;
   
      compare1 = abs(compare1);
      compare2 = abs(compare2);
   
   if (compare1 < compare2) return mid + 1;
   return mid;
}

int giantStepper (double key, double A[], int n, int & numCompares) {
   if (n < 1) return -1;
   numCompares = 0;
   
   int step = sqrt(n);     // Step for search
   int largerIndex = 0;    // Index of first number larger than key
   
   for (int i = step - 1; i < n;) {
      numCompares += 1;
      
      if (A[i] == key) return i;
      if (A[i] > key) {
         largerIndex = i;
         break;
      }
      
      i += step;
      if (i >= n) largerIndex = n - 1;
   }
   
   int j = largerIndex;

   while (A[j] > key && j > 0) {
      j -= 1;
   }
   
 
   if (A[j] == key) return j;
   
 
   double compare1 = A[j + 1] - key;
   double compare2 = A[j] - key;
   
   compare1 = abs(compare1);
   compare2 = abs(compare2);
   
   if (compare1 < compare2) return j + 1;
   return j;
}



