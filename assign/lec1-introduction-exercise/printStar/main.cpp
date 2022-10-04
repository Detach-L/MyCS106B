/**
 * We will learn more about hashing later this quarter!
 */

#include <iostream>
#include "console.h"
#include <cmath>
//#include "simpio.h"  // for getLine
using std::cout;
using std::cin;
using std::endl;

void printStar(int n);
void printNum(int n);

int main() {
    int n;
    cin >> n;
//    printStar(n);
    printNum(n);
    return 0;
}

/*
* n: the num of layer
* for x in [1-n, n-1], y in [0, n-1]:
*   if n is odd:
*       print "*" when |x| + |y| < n and sum is even
*   if n is even:
*       print "*" when |x| + |y| < n and sum is odd
*/
void printStar(int n){
    for(int i=n-1; i>=0; i--){ // for y: (n-1)-->0
        for(int j=1-n; j<=n-1; j++){ // for x: -(n-1)-->+(n-1)
            int sum = abs(i) + abs(j);
            if(sum < n && (n+sum)%2 == 1){  // if n is odd, sum must be even and vice versa
                cout << "*";
            }
            else{
                cout << " ";
            }
        }
        cout << endl;
    }
}

void printNum(int n){
    for(int i=n-1; i>=0; i--){ // for y: (n-1)-->0
        for(int j=1-n; j<=n-1; j++){ // for x: -(n-1)-->+(n-1)
            if(i > 0){  // for [1, n-1] layer
                int sum = abs(i) + abs(j);
                if(sum == n-1){
                    cout << (n-i);
                }
                else{
                    cout << " ";
                }
            }
            else{  // for last layer
                if((n+abs(j))%2 == 1){
                    cout << n;
                }
                else{
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
}


