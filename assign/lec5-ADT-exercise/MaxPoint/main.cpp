/**
 * We will learn more about hashing later this quarter!
 */

#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "simpio.h"  // for getLine
#include "maxPoint.h"
using namespace std;


int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
            return 0;
        }

    Vector<int> v = { 1, 3, 7 };
    int maxValue = maxOf(v);
    cout << "The max value of vector:  " << maxValue  << endl;

    cout << endl << "main() completed." << endl;
    return 0;
}


