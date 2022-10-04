#include "maxPoint.h"
#include "testing/SimpleTest.h"
#include "random.h"

int maxOf(Vector<int> elems) {
    // edge case
    if (elems.size() == 1)
        return elems[0];

    // pick the first point
    int first = elems.get(0);

    // get the max point in the rest (someone do it for you)
    Vector<int> rest = elems.subList(1);
    int maxOfRest = maxOf(rest);

    // compare your point with the max point in the rest
    return first > maxOfRest ? first : maxOfRest;
}

int maxOfBinarySearch(Vector<int> elems) {
    // edge case
    if (elems.size() == 1)
        return elems[0];

    // Divide the array in half
    int mid = elems.size() / 2;
    Vector<int> left = elems.subList(0, mid);
    Vector<int> right = elems.subList(mid);

    // get the max point in the rest (someone do it for you)
    int maxOfLeft = maxOfBinarySearch(left);
    int maxOfRight = maxOfBinarySearch(right);

    // compare your point with the max point in the rest
    return maxOfLeft > maxOfRight ? maxOfLeft : maxOfRight;
}

int maxOfBinarySearch(Vector<int>& elems, int start, int end) {
    // edge case
    if (end - start == 1)
        return elems[start];

    // Divide the array in half by index
    int mid = (start + end) / 2;
    // get the max point in the rest (someone do it for you)
    int maxOfLeft = maxOfBinarySearch(elems, start, mid);
    int maxOfRight = maxOfBinarySearch(elems, mid, end);

    // compare your point with the max point in the rest
    return maxOfLeft > maxOfRight ? maxOfLeft : maxOfRight;
}

// TODO: add your student test cases here

STUDENT_TEST("Test maxOf") {
    Vector<int> v = {1, 3, 9, 5};
    EXPECT_EQUAL(maxOf(v), 9);
    v = {3};
    EXPECT_EQUAL(maxOf(v), 3);
    v = {3, 1};
    EXPECT_EQUAL(maxOfBinarySearch(v), 3);
    v = {1, 3};
    EXPECT_EQUAL(maxOfBinarySearch(v), 3);
}

STUDENT_TEST("Test maxOfBinarySearch") {
    Vector<int> v = {1, 3, 9, 5};
    EXPECT_EQUAL(maxOfBinarySearch(v), 9);
    v = {3};
    EXPECT_EQUAL(maxOfBinarySearch(v), 3);
    v = {3, 1};
    EXPECT_EQUAL(maxOfBinarySearch(v), 3);
    v = {1, 3};
    EXPECT_EQUAL(maxOfBinarySearch(v), 3);
}

STUDENT_TEST("Test maxOfBinarySearch") {
    Vector<int> v = {1, 3, 9, 5, 7};
    EXPECT_EQUAL(maxOfBinarySearch(v, 0, v.size()), 9);
    v = {3};
    EXPECT_EQUAL(maxOfBinarySearch(v, 0, v.size()), 3);
    v = {3, 1};
    EXPECT_EQUAL(maxOfBinarySearch(v, 0, v.size()), 3);
    v = {1, 3};
    EXPECT_EQUAL(maxOfBinarySearch(v, 0, v.size()), 3);
}

STUDENT_TEST("Time trials of maxOf on doubling input sizes") {
    Vector<int> v;
    for(int i=0; i < 10000; i++){
        v.add(randomInteger(0, 10));
    }
    TIME_OPERATION(v.size(), maxOf(v));
}

STUDENT_TEST("Time trials of maxOfBinarySearch on doubling input sizes") {
    Vector<int> v;
    for(int i=0; i < 10000; i++){
        v.add(randomInteger(0, 10));
    }
    TIME_OPERATION(v.size(), maxOfBinarySearch(v));
}

STUDENT_TEST("Time trials of maxOfBinarySearch on doubling input sizes") {
    Vector<int> v;
    for(int i=0; i < 10000; i++){
        v.add(randomInteger(0, 10));
    }
    TIME_OPERATION(v.size(), maxOfBinarySearch(v, 0, v.size()));
}
