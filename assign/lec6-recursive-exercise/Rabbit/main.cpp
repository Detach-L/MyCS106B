/**
 * We will learn more about hashing later this quarter!
 */

#include <iostream>
#include "console.h"
#include "simpio.h"  // for getLine
using namespace std;

int fib(int n) {
    if(n < 2)
        return n;
    return fib(n - 1) + fib(n - 2);
}

int fibDie(int n) {
    if(n < 2)
        return n;
    if(n == 2)
        return 1;
    // n>4时, 第n个月兔子数 = n-1个月的 + n-2个月的 - n-3个月的
    return fibDie(n - 1) + fibDie(n - 2) - fibDie(n - 3);
}


int main() {
    int n = 10;
//    for (int i = 0; i < n; ++i) {
//        cout << "fib(" << i << ")=" << fib(i) << endl;
//    }

    for (int i = 0; i < n; ++i) {
        cout << "fibDie(" << i << ")=" << fibDie(i) << endl;
    }

    return 0;
}


