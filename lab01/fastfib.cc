using namespace std;
#include <iostream>
#include <fstream>

double FastFibRecCount (int n, int & totalAdds, int & totalCalls);
void FFRC2( int n, double &Fn_1, double & Fn, int & totalAdds, int & totalCalls);
double FibLoopCount (int n, int & totalAdds, int & totalCalls);

const int MAX_N = 30;
const int MAX_CALLS = 100000;

int main () {
    // cout << "Enter single positive integer: ";
    int k = MAX_N;
    // cout << endl;
    
    ofstream myfile ("test2.csv");
    if (myfile.is_open()) {
    
        for (int i = 1; i < k; ++i) {
                
            int TA1 = 0; //times added in iterative function
            int TC1 = 0; //times called
        
            int TA2 = 0; //times added in recursive function
            int TC2 = 0; //times called
            
            if (FibLoopCount(i, TA1, TC1) == FastFibRecCount(i, TA2, TC2)){
                // cout << TA1 << " " << TC1 << endl;
                // cout << TA2 << " " << TC2 << endl << endl; 
                if (TC1 > MAX_CALLS || TC2 > MAX_CALLS) return 0;
                myfile << i << ",";
                myfile << TA1 << "," << TC1 << ",";
                myfile << TA2 << "," << TC2 << endl;
            }
            else {
                cout << "Error: " << FibLoopCount(i, TA1, TC1) << " " << FastFibRecCount(i, TA2, TC2);
            }
        }
    }
    else cout << "Unable to open file" << endl;


    
    return 0;
}

double FastFibRecCount (int n, int & totalAdds, int & totalCalls) {
    double twoBack = 0; //One from number being found
    double oneBack = 1; //Number being found
    if (n == 0) return 0;
    if (n == 1) return 1;
    FFRC2(n, twoBack, oneBack, totalAdds, totalCalls);
    return oneBack;
    
}

void FFRC2 (int n, double & Fn_1, double & Fn, int & totalAdds, int & totalCalls) {
    n -= 1;
    if (n > 0) {
        totalAdds += 1;
        totalCalls += 1;
        int temp = Fn;
        Fn += Fn_1;
        Fn_1 = temp;
        FFRC2(n, Fn_1, Fn, totalAdds, totalCalls);
    }
    return;
}

//Taken from fibcount.cc
double FibLoopCount (int n, int & totalAdds, int & totalCalls){
    int num[n + 1];
    num[0] = 0;
    num[1] = 1;

        for (int i = 2; i <= n; ++i) {
            num[i] = num[i - 1] + num[i - 2]; 
            totalAdds += 1;
        }
    
    return num[n];
}