#include <iostream>
using namespace std;
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

double bisect (double (*f) (double x), double a, double b, double eps, int & numCompares);

double sec_plus (double (*f) (double x), double a, double b, double eps, int & numCompares);

double f(double x);

double d(double x);

double g(double x);

int main() {
    // ios::fmtflags old_settings = cout.flags();  // Save pervious format flags
    int old_precision = cout.precision();       // Save previous precision setting
    
    cout.setf(ios::fixed, ios::floatfield);     
    
    // cout << "Function 1: f(x) = x^2 - 2\n\n";
    cout << "Using Bisect Method: \n";
    cout << "Functions with precision 1e-6: \n";
    cout.precision(6);
    
    int numCompares = 0;
    cout << setw(0) << "f(x) = x^2 - 2" << setw(20) <<  "[1,2] " << setw(19) << bisect (*f, 1, 2, 1e-6, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl;
    
    cout.precision(6);
    numCompares = 0;
    cout << setw(0) << "d(x) = x^2 - 3" << setw(21) <<  "[1,10] " << setw(18) << bisect (*d, 1, 10, 1e-6, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl;
    
    cout.precision(6);
    numCompares = 0;
    cout << setw(0) << "g(x) = 2x^3 - 4x^2 + 3x" << setw(12) <<  "[-1,1] " << setw(18) << bisect (*g, -1, 1, 1e-6, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl;
    
    cout << "Functions with precision 1e-15: \n";
    
    cout.precision(15);
    
    cout << setw(0) << "f(x) = x^2 - 2" << setw(20) <<  "[1,2] " << setw(19) << bisect (*f, 1, 2, 1e-15, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl;
    
    cout.precision(15);
    numCompares = 0;
    cout << setw(0) << "d(x) = x^2 - 3" << setw(21) <<  "[1,10] " << setw(18) << bisect (*d, 1, 10, 1e-15, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl;
    
    cout.precision(15);
    numCompares = 0;
    cout << setw(0) << "g(x) = 2x^3 - 4x^2 + 3x" << setw(12) <<  "[-1,1] " << setw(18) << bisect (*g, -1, 1, 1e-15, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl;
    
    cout << "\nUsing Secant Method: " << endl;
    cout << "Functions with precision 1e-6: \n";
    
    cout.precision(6);
    numCompares = 0;
    cout << setw(0) << "f(x) = x^2 - 2" << setw(20) <<  "[1,2] " << setw(19) << sec_plus (*f, 1, 2, 1e-6, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl << endl;
    
    cout.precision(6);
    numCompares = 0;
    cout << setw(0) << "d(x) = x^2 - 3" << setw(21) <<  "[1,10] " << setw(18) << sec_plus (*d, 1, 10, 1e-6, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl << endl;
    
    cout.precision(6);
    numCompares = 0;
    cout << setw(0) << "g(x) = 2x^3 - 4x^2 + 3x" << setw(12) <<  "[-1,1] " << setw(18) << sec_plus (*g, -1, 1, 1e-6, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl << endl;
    
    cout << "Functions with precision 1e-15: \n";
    
    cout.precision(15);
    numCompares = 0;
    cout << setw(0) << "f(x) = x^2 - 2" << setw(20) <<  "[1,2] " << setw(19) << sec_plus (*f, 1, 2, 1e-15, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl << endl;
    
    cout.precision(15);
    numCompares = 0;
    cout << setw(0) << "d(x) = x^2 - 3" << setw(21) <<  "[1,10] " << setw(18) << sec_plus (*d, 1, 10, 1e-15, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl << endl;
    
    cout.precision(15);
    numCompares = 0;
    cout << setw(0) << "g(x) = 2x^3 - 4x^2 + 3x" << setw(12) <<  "[-1,1] " << setw(18) << sec_plus (*g, -1, 1, 1e-15, numCompares) << ' ';
    cout.precision(old_precision);
    cout << "Compares: " << numCompares << endl;
    return 0;
}

double bisect (double (*f) (double x), double a, double b, double eps, int & numCompares) {
        
        numCompares += 1;
        double m = a + ((b - a) / 2);  
        
        // cout << m << endl;
        if (f(m) == 0) return m;
        
        double largest = max(fabs(a), fabs(b));
        // if( fabs(f(m)) < (largest * eps) ) return m;
        if( fabs(a - b) < (largest * eps) ) return m;

        
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
    x = x - 3;
    return x;
}

double g(double x) {
    return (2 * pow(x,3)) - (4 * pow(x,2)) + (3 * x);
}

double sec_plus (double (*f) (double x), double a, double b, double eps, int & numCompares){
    
    int old_precision = cout.precision();
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    double fA = 0;
    double fB = 0;
    double fM = 0;
    double m = 0;
    int counterA = 0;
    int counterB = 0;
    
    while(1) {
        numCompares += 1;
        if (numCompares == 1) {
            m = ((f(b) * a) - (f(a) * b)) / (f(b) - f(a));
            fA = f(a);
            fB = f(b);
            fM = f(m);
        }
        else {
            m = ((fB * a) - (fA * b)) / (fB - fA);
            fM = f(m);
        }
        
        // cout << fixed << setprecision(2);
        cout << setw(5) << a << ' ' << setw(5) << fA << ' ' << setw(5) << b << ' ' << setw(5) << fB << ' ' << setw(5) << m << ' ' << setw(5) << fM << endl;
        
        double largest = max(fabs(a), fabs(b));
        if( fabs(fM) < (largest * eps) ) {
            cout.precision(old_precision);
            return m;
        }
        
        if ((fA < 0 && fM < 0) || (fA > 0 && fM > 0)) {
            a = m;
            fA = f(a);
            counterA += 1;
            counterB = 0;
            if (counterA >= 2) {
                fB = fB / 2;
            }
        }
        else {
            b = m;
            fB = f(b);
            counterB += 1;
            counterA = 0;
            if (counterB >= 2) {
                fA = fA / 2;
            }
        }
            
        if (f(m) == 0) {
            cout.precision(old_precision);
            return m;
        }
    }
}
