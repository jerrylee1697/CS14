using namespace std;
#include <iostream>

double FibLoop (int n); //returns the nth element of the fibinacci series
double FibRec (int n);


int main () {
    cout << "Enter single positive integer: ";
    int k = 0;
    cin >> k;
    cout << endl;
    
    for (int i = 0; i < k; ++i) {
        if (FibLoop(i) == FibRec(i)){
            cout << i << " ";
            cout << FibRec(i) << endl;    
        }
        else {
            cout << "Error: " << FibLoop(i) << " " << FibRec(i) << endl;
            break;
        }
    }
    
    return 0;
}

double FibLoop (int n) {
    int num[n + 1];
    num[0] = 0;
    num[1] = 1;

        for (int i = 2; i <= n; ++i) {
            num[i] = num[i - 1] + num[i - 2]; 
        }
    
    return num[n];
}

double FibRec (int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return FibRec(n - 1) + FibRec(n - 2);
}
