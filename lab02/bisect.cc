#include <iostream>
using namespace std;
#include <fstream>
#include <algorithm>
#include <cmath>

double bisect (double (*f) (double x), double a, double b, double eps, int & numCompares);

double f(double x);

double d(double x);

int main() {
    cout << "Function 1: f(x) = x^2 - 2\n\n";
    
    int numCompares = 0;
    cout << "Single precision (1e-6): ";
    cout << bisect (*f, 1, 2, 1e-6, numCompares) << endl;
    cout << "numCompares: " << numCompares << endl << endl;
    
    numCompares = 0;
    cout << "Double precision (1e-15): ";
    cout << bisect (*f, 1, 2, 1e-15, numCompares) << endl;
    cout << "numCompares: " << numCompares << endl << endl;
    
    cout << "Function 2: f(x) = x^2 - 10\n\n";
    numCompares = 0; 
    cout << "Single precision (1e-6): ";
    cout << bisect (*d, 1, 10, 1e-6, numCompares) << endl;
    cout << "numCompares: " << numCompares << endl << endl;
    
    numCompares = 0; 
    cout << "Double precision (1e-15): ";
    cout << bisect (*d, 1, 10, 1e-15, numCompares) << endl;
    cout << "numCompares: " << numCompares << endl;
    return 0;
}

double bisect (double (*f) (double x), double a, double b, double eps, int & numCompares) {
        
        numCompares += 1;
        double m = a + ((b - a) / 2);  
        
        if (f(m) == 0) return m;
        
        double largest = max(fabs(a), fabs(b));
        if( fabs(f(m)) < (largest * eps) ) return m;
        
        if ((f(a) < 0 && f(m) < 0) || (f(a) > 0 && f(m) > 0)) 
            a = m;
        else {
            b = m;
        }
        
    return bisect(*f, a, b, eps, numCompares);
}

double f(double x) {
    x = pow(x,2);
    x = x - 2;
    return x;
}

double d(double x) {
    x = pow(x,2);
    x = x - 10;
    return x;
}