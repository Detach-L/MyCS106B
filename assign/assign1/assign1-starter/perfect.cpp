/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/*  This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to int(sqrt(n)), testing for a
 * zero remainder from the division.
 *
 * Note: This is an improve version of `divisorSum`.
 */
long smarterSum(long n) {
    long total = 0;
    if (n <= 0)  // negtive can't excute sqrt operation!
        return total;
    for (long divisor = 1; divisor < sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor;  // divisor
            total += n/divisor;  // pair greater divisor
        }
    }
    if (static_cast<int>(sqrt(n)) == sqrt(n))  // like 25, sqrt(25)=5 is an divisor of 25;
        total += sqrt(n);
    return total - n;  // if n>1, will add an extra n; if n==1, int(sqrt(n)) == sqrt(n), will add an extra 1;
}

/* Same as isPerfect but call the smarterSum rather than divisorSum.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* Same as findPerfects.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* It takes one argument `stop` and searches the range 1 to `stop`,
 * using Mersenne prime to check whether a number is perfect and if so,
 * printing it to the console.
 */
long findNthPerfectEuclid(long n) {
    int k = 1;
    int count= 0;
    while(true) {
        int m = pow(2, k) - 1;
        if(smarterSum(m) == 1)  // m is prime
            count++;
        if(count == n)
            return pow(2, k-1) * (pow(2, k) - 1);
        k++;
    }
    return 0;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here

STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(25000, findPerfects(25000));  // 1.046s
    TIME_OPERATION(50000, findPerfects(50000));  // 3.727s
    TIME_OPERATION(100000, findPerfects(100000));  // 13.419s
    TIME_OPERATION(200000, findPerfects(200000));  // 50.798s
}

STUDENT_TEST("Test oddballs: negtive numbers are not perfect") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-8128));
    EXPECT(!isPerfect(-33550336));
}

STUDENT_TEST("Confirm divisorSum of small inputs when total is initialized to 1") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

STUDENT_TEST("Confirm smarterSum of some inputs") {
    EXPECT_EQUAL(smarterSum(-25), 0);
    EXPECT_EQUAL(smarterSum(0), 0);
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
    EXPECT_EQUAL(smarterSum(25), 6);
    EXPECT_EQUAL(smarterSum(28), 28);
}

STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(562500, findPerfectsSmarter(562500));  // 3.371s
    TIME_OPERATION(1125000, findPerfectsSmarter(1125000));  // 8.773s
    TIME_OPERATION(2250000, findPerfectsSmarter(2250000));  // 23.196s
    TIME_OPERATION(4500000, findPerfectsSmarter(4500000));  // 62.892s
}

STUDENT_TEST("Test findNthPerfectEuclid") {
    EXPECT(isPerfect(findNthPerfectEuclid(1)));
    EXPECT(isPerfect(findNthPerfectEuclid(2)));
    EXPECT(isPerfect(findNthPerfectEuclid(4)));
    EXPECT(isPerfect(findNthPerfectEuclid(5)));
//    EXPECT(isPerfect(findNthPerfectEuclid(6)));
}
