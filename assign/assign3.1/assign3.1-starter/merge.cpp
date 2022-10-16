/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

/*
 * 验证一个队列是否是升序序列
 */
bool isQueueSortByAscending(Queue<int> a) {
    if(a.isEmpty())
        return true;
    while(true) {
        int first = a.dequeue();
        if(!a.isEmpty()) {
            if(first > a.peek())
                return false;
        } else {
            return true;
        }
    }
}

/* * * * * * STUDENT Test Cases * * * * * */
STUDENT_TEST("STUDENT_TEST check a queue whether sort by ascending") {
    Queue<int> a = {1, 2, 4, 5};
    EXPECT(isQueueSortByAscending(a));
    a = {1, 2, 2, 4};
    EXPECT(isQueueSortByAscending(a));
    a = {2, 2, 1, 4};
    EXPECT(!isQueueSortByAscending(a));
}

/*
 * 使用迭代的方式合并两个序列
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    // a和b都不为空合并两者
    int resultLast;
    while(!a.isEmpty() && !b.isEmpty()) {
        // a.peek()前必须要判断a.isEmpty(), 特别是前面执行了a.dequeue()后要判断
        // 所以把这个判断移到下面两个a/b.dequeue()前面了, 只需执行一个!result.isEmpty()判断
        if(!result.isEmpty()) {
            if(resultLast > a.peek() || result.peek() > b.peek())
                error("队列不是升序序列!");
        }
        if(a.peek() < b.peek()) {
            resultLast = a.peek();
            result.enqueue(a.peek());
            a.dequeue();
        } else {
            resultLast = b.peek();
            result.enqueue(b.peek());
            b.dequeue();
        }
    }
    // 如果a比b长, 合并a剩余元素
    while(!a.isEmpty()) {
        if(!result.isEmpty()) {  // 得加上这个判断, 因为b是空的的话, resultLast没有赋值
            if(resultLast > a.peek())
                error("队列不是升序序列!");
        }
        resultLast = a.peek();
        result.enqueue(a.peek());
        a.dequeue();
    }
    // 如果b比a长, 合并b剩余元素
    while(!b.isEmpty()) {
        if(!result.isEmpty()) {
            if(resultLast > b.peek())
                error("队列不是升序序列!");
        }
        resultLast = b.peek();
        result.enqueue(b.peek());
        b.dequeue();
    }
    return result;
}

/* * * * * * STUDENT Test Cases * * * * * */
STUDENT_TEST("STUDENT_TEST binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {};
    b = {};
    expected = {};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {};
    b = {1, 3, 3};
    expected = {1, 3, 3};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {1, 2, 3, 4, 5, 6, 6, 7};
    b = {9};
    expected = {1, 2, 3, 4, 5, 6, 6, 7, 9};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);

    a = {1, 2, 3, 4, 5, 6, 6, 7};
    b = {1};
    expected = {1, 1, 2, 3, 4, 5, 6, 6, 7};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("STUDENT_TEST binaryMerge with order check") {
    Queue<int> a = {1, 2, 2};
    Queue<int> b = {1, 3, 2};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));

    a = {1, 2};
    b = {1, 3, 3, 3, 5, 1};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("STUDENT_TEST Time binaryMerge operation") {
    int n = 20000;
    for (int i=1; i<=4; ++i) {
        Queue<int> a = createSequence(n);
        Queue<int> b = createSequence(n);
        TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
        n *= 2;
    }
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/* * * * * * STUDENT Test Cases * * * * * */
STUDENT_TEST("STUDENT_TEST naiveMultiMerge, empty collection") {
    Vector<Queue<int>> all = {{},
                             {},
                             {},
                             {}
                            };
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);

    all = {};
    expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("STUDENT_TEST Time naiveMultiMerge operation of k") {
    int k = 10;
    int n = 2000;
    Vector<Queue<int>> all;
    for (int j=1; j<=4; ++j) {
        for (int i=1; i<=k; ++i) {
            all.add(createSequence(n));
        }
        TIME_OPERATION(n*k, naiveMultiMerge(all));
        k *= 2;
    }
}

STUDENT_TEST("STUDENT_TEST Time naiveMultiMerge operation of n") {
    int k = 5;
    int n = 1000;
    Vector<Queue<int>> all;
    for (int j=1; j<=4; ++j) {
        for (int i=1; i<=k; ++i) {
            all.add(createSequence(n));
        }
        TIME_OPERATION(n*k, naiveMultiMerge(all));
        n *= 2;
    }
}

/*
 * 利用递归加上引用实现多路归并排序
 */
Queue<int> recMultiMergeHelper(Vector<Queue<int>>& all, int start, int end) {
    Queue<int> result;
    // edge case
    if(end - start == 0)
        return result;
    if(end - start == 1)
        return all[start];
    // rest
    int mid = (start + end) / 2;
    Queue<int> left = recMultiMergeHelper(all, start, mid);
    Queue<int> right = recMultiMergeHelper(all, mid, end);
    result = binaryMerge(left, right);
    return result;
}

Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    return recMultiMergeHelper(all, 0, all.size());
}

/* * * * * * STUDENT Test Cases * * * * * */
STUDENT_TEST("STUDENT_TEST recMultiMerge, compare to naiveMultiMerge") {
    for (int i=0; i<4; ++i) {
        int n = 20 + i;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(n);
        distribute(input, all);
        EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
    }
}

/*
 * 利用递归实现多路归并排序
 */
Queue<int> recMultiMergeNavie(Vector<Queue<int>>& all) {
    Queue<int> result;
    // edge case
    if(all.size() == 0)
        return result;
    if(all.size() == 1)
        return all[0];
    // rest
    int mid = all.size() / 2;
    Vector<Queue<int>> tmp = all.subList(0, mid);
    Queue<int> left = recMultiMerge(tmp);  // 不知道为什么直接将subList放进去报错, 是&的关系
    //Queue<int> left1 = recMultiMerge(all.subList(0, mid));
    tmp = all.subList(mid);
    Queue<int> right = recMultiMerge(tmp);
    result = binaryMerge(left, right);
    return result;
}

/* * * * * * STUDENT Test Cases * * * * * */
STUDENT_TEST("STUDENT_TEST recMultiMerge, compare to naiveMultiMerge") {
    for (int i=0; i<4; ++i) {
        int n = 20 + i;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(n);
        distribute(input, all);
        EXPECT_EQUAL(recMultiMergeNavie(all), naiveMultiMerge(all));
    }
}


/* * * * * * Test Cases * * * * * */

//Queue<int> createSequence(int size);
//void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
