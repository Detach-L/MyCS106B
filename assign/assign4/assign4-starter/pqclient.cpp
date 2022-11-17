#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "testing/SimpleTest.h"
using namespace std;

/* TODO: Refer to pqclient.h for more information about what this function does, then
 * delete this comment.
 */
void pqSort(Vector<DataPoint>& v) {
    PQArray pq;

    /* Using the Priority Queue data structure as a tool to sort, neat! */

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; !pq.isEmpty(); i++) {
        v[i] = pq.dequeue();
    }
}

/*
 * 从大规模数据流中获取前k个最大值
 *     - 如果流包含元素大于 k 个，则向量包含具有最大值的 k 个元素
 *     - 如果流包含元素少于 k 个，则向量包含流中的所有元素
 *     - k 必须大于 0
 *     - 在所有情况下，向量中的元素都按优先级值降序排列
 */
Vector<DataPoint> topK(istream& stream, int k) {
//    PQArray pq;
    PQHeap pq;
    DataPoint cur;
    while(stream >> cur) {
        if(pq.size() < k) {  // 小于7个元素直接入队, 此处是<不是<=;
            pq.enqueue(cur);
        } else {  // 多于7个元素, 由于要最大的k个, 先出队优先级最小的(最紧急的), 再入队
            DataPoint minDP = pq.peek();
            if(minDP.priority < cur.priority) {  // 只有当前最小元素的优先级小于cur的才出队
                pq.dequeue();
                pq.enqueue(cur);
            }
        }
    }
    if(pq.size() <= 0) {  // stream里没有数据, 直接返回空
        return {};
    } else {
        // 可能会有元素个数小于k的情况, 所以result大小应该是pq.size()而不是k
        Vector<DataPoint> result(pq.size());
        for(int i=pq.size()-1; i>=0; --i) {
            result[i] = pq.dequeue();  // 先出队优先级小的, 所以要倒着放
        }
        return result;
    }
}

Vector<DataPoint> topKArray(istream& stream, int k) {
    PQArray pq;
    DataPoint cur;
    while(stream >> cur) {
        if(pq.size() < k) {  // 小于7个元素直接入队, 此处是<不是<=;
            pq.enqueue(cur);
        } else {  // 多于7个元素, 由于要最大的k个, 先出队优先级最小的(最紧急的), 再入队
            DataPoint minDP = pq.peek();
            if(minDP.priority < cur.priority) {  // 只有当前最小元素的优先级小于cur的才出队
                pq.dequeue();
                pq.enqueue(cur);
            }
        }
    }
    if(pq.size() <= 0) {  // stream里没有数据, 直接返回空
        return {};
    } else {
        // 可能会有元素个数小于k的情况, 所以result大小应该是pq.size()而不是k
        Vector<DataPoint> result(pq.size());
        for(int i=pq.size()-1; i>=0; --i) {
            result[i] = pq.dequeue();  // 先出队优先级小的, 所以要倒着放
        }
        return result;
    }
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", double(i) };
        result << pt;
    }
    return result;
}

/* Helper function to fill vector with n random DataPoints. */
void fillVector(Vector<DataPoint>& vec, int n) {
    vec.clear();
    for (int i = 0; i < n; i++) {
        DataPoint pt = { "", randomReal(0, 100) };
        vec.add(pt);
    }
}

/* TODO: Add your own custom tests here! */
//STUDENT_TEST("pqSort: time trial") {
//    int n = 10000;
//    for (int i=0; i<4; ++i) {
//        Vector<DataPoint> v;
//        fillVector(v, n);
//        TIME_OPERATION(n, pqSort(v));
//        n *= 2;
//    }
//}











/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("pqSort: vector of random elements") {
    setRandomSeed(137); // why might it be a good idea to set seed here?

    Vector<DataPoint> input;
    Vector<double> expected;
    fillVector(input, 100);
    for (DataPoint dp : input) {
        expected.add(dp.priority);
    }
    expected.sort();

    pqSort(input);
    for (int i = 0; i < input.size(); i++) {
        EXPECT_EQUAL(input[i].priority, expected[i]);
    }
}

PROVIDED_TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("topK: stream 0 elements, ask for top 1") {
    stringstream emptyStream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(emptyStream, 1), expected);
}

PROVIDED_TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "" , 1 } });
    Vector<DataPoint> expected = { { "" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: small hand-constructed input") {
    Vector<DataPoint> input = { { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 } };

    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { { "D", 4 }, { "C", 3 } };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 } };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 1") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = { { "" , kMany } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 5") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = {
        { "" , kMany     },
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stress test, many elements, random values") {
    Vector<double> expected;
    Vector<DataPoint> points;
    fillVector(points, 10000);
//    fillVector(points, 20);
    for (DataPoint dp : points) {
        expected.add(dp.priority);
    }
    sort(expected.begin(), expected.end(), greater<double>());

    stringstream stream = asStream(points);
    int k = 10;
    Vector<DataPoint> result = topK(stream, k);
    EXPECT_EQUAL(result.size(), k);
    for (int i = 0; i < k; i++) {
        EXPECT_EQUAL(result[i].priority, expected[i]);
    }
}

PROVIDED_TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}

PROVIDED_TEST("topK: time trial") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("topKHeap: time trial for n") {
    int startSize = 2000000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("topKHeap: time trial for k") {
    int startSize = 10000;
    int n = 400000;
    for (int k = startSize; k < 10*startSize; k *= 2) {
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(k, topK(stream, k));
    }
}

STUDENT_TEST("topKArray: time trial for n") {
    int startSize = 2000000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topKArray(stream, k));
    }
}

STUDENT_TEST("topKArray: time trial for k") {
    int startSize = 10000;
    int n = 400000;
    for (int k = startSize; k < 10*startSize; k *= 2) {
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(k, topKArray(stream, k));
    }
}

