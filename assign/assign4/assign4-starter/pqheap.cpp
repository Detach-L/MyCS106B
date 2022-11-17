#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * 初始化队列成员函数;
 * 初始分配内存大小为10, 也即创建一个一开始能存放十个元素的动态数组;
 * 动态数组中所有元素会被初始化为0;
 */
PQHeap::PQHeap() {
    this->numAllocated = INITIAL_CAPACITY;
    this->numFilled = 0;
    this->elements = new DataPoint[this->numAllocated]();  // ()初始化成0
}

/*
 * 释放为队列分配的空间;
 * 主要是释放动态数组;
 */
PQHeap::~PQHeap() {
    delete[] this->elements;
}

/*
 * 将元素根据优先级插入队列;
 * 以二叉堆存储队列, 若优先级值越小表示越紧急时, 则堆顶元素优先级值最小;
 *     1.若队列已满, 需要扩容
 *     2.若队列不满, 先将元素插入二叉堆尾部, 然后根据二叉堆性质上浮插入元素
 * 不考虑扩容复杂度O(logn), 扩容复杂度为O(n), 总体O(logn)+O(n)
 */
void PQHeap::enqueue(DataPoint elem) {
    if(this->isFull())
        this->expand(2);
    this->insertToArray(elem);
}

STUDENT_TEST("测试入队相关函数以及检验状态函数") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
}

/*
 * 判断队列分配内存是否已用完;
 * 用完则返回true, 否则返回false
 */
bool PQHeap::isFull() const{
    return size() == this->numAllocated;
}

/*
 * 队列分配内存已用完时扩容, multiple表示扩容倍数;
 * 扩容需要先移动当前元素到新队列, 然后删除旧队列;
 */
void PQHeap::expand(int multiple){
    DataPoint* oldArray = this->elements;
    int oldSize = this->numAllocated;  // 扩容时等价于int arraySize = size();
    this->numAllocated *= multiple;
    this->elements = new DataPoint[numAllocated]();
    if(NULL != this->elements) {
        for(int i=0; i<oldSize; ++i) {
            this->elements[i] = oldArray[i];
        }
        delete[] oldArray;
    } else {
        error("内存扩展失败!");
    }
}

/*
 * 将元素插入二叉堆尾部,
 * 然后根据下述条件上浮插入元素:
 *     若插入节点比父节点小, 则交换两节点位置, 然后重复此操作;
 *     若插入节点比父节点大, 则退出;
 * O(logn)
 */
void PQHeap::insertToArray(const DataPoint& elem){
    int curIndex = size();  // 此处不用减1
    this->elements[curIndex] = elem;
    this->numFilled++;  // 此操作先后无所谓, 反正自底向上不用判断size()
    while(true) {
        int parentIndex = this->getParentIndex(curIndex);
        if(parentIndex != NONE &&
            this->elements[curIndex].priority < this->elements[parentIndex].priority){
            swap(this->elements[curIndex], this->elements[parentIndex]);
            curIndex = parentIndex;
        } else {
            break;
        }
    }
}

/**
 * 此交换函数有问题, 引用使用错误;
 * tmp引用了indexA之后, 如果给indexA赋值, 会同步改变tmp值
 */
//void PQHeap::swap(DataPoint& indexA, DataPoint& indexB){
//    DataPoint& tmp = indexA;
//    indexA = indexB;
//    indexB = tmp;
//}
void PQHeap::swap(DataPoint& indexA, DataPoint& indexB){
    DataPoint tmp = indexA;
    indexA = indexB;
    indexB = tmp;
}

/*
 * 优先值越小表示越紧急时, 返回队列优先值最小的元素;
 * 对于数组形式存储的二叉堆, 返回队首元素;
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    return this->elements[0];
}

/*
 * 以二叉堆存储队列, 若优先级值越小表示越紧急时, 则堆顶元素优先级值最小;
 * 删除堆顶元素, 也即数组下标0元素;
 * 根据二叉堆性质重建二叉堆;
 * O(logn)
 */
DataPoint PQHeap::dequeue() {
    DataPoint front = this->peek();
    this->deleteFromArray();  // 经过peek后队列至少有一个元素, 不用在此函数判断了
    return front;
}

STUDENT_TEST("测试出队相关函数") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

/*
 * 删除堆顶元素,
 * 将堆中最后一个元素移动到堆顶,
 * 然后根据下述条件下沉移动的元素:
 *   若移动元素比其左右子结点`最小`的那个大, 则交换两节点位置, 然后重复此操作;
 *   否则退出;
 * O(logn)
 */
void PQHeap::deleteFromArray(){
    // 经过peek后队列至少有一个元素, 不用在此函数判断了
    int curIndex = 0;
    this->elements[0] = this->elements[size() - 1];
    // 此操作必须在上一句后进行, 不然影响size()判断; 并且必须在getLeftChildIndex之前判断
    this->numFilled--;
    while(true){
        int leftChild = this->getLeftChildIndex(curIndex);
        int rightChild = this->getRightChildIndex(curIndex);
        int childIndex;
        if(leftChild != NONE && rightChild != NONE) {  // 有左右孩子取最小的那个
            childIndex = this->getMinChildIndex(leftChild, rightChild);
        }else if(leftChild != NONE) {
            childIndex = leftChild;
//        }else if(rightChild != NONE) {  // 按照二叉树插入, 没有左孩子一定没有右孩子
//            childIndex = rightChild;
        }else {  // 没有子结点直接退出
            break;
        }
        if(this->elements[curIndex].priority > this->elements[childIndex].priority) {
            this->swap(this->elements[curIndex], this->elements[childIndex]);
            curIndex = childIndex;
        }else {  // 移动元素优先级值小于左右节点, 已满足二叉堆性质, 直接退出
            break;
        }
    }
}

int PQHeap::getMinChildIndex(int indexLeft, int indexRight){
    if(this->elements[indexLeft].priority <= this->elements[indexRight].priority)
        return indexLeft;
    else
        return indexRight;
}

/*
 * 队列中若没有元素返回true, 否则返回false;
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * 返回队列元素个数;
 */
int PQHeap::size() const {
    return this->numFilled;
}

/*
 * 将队列清空;
 * 清空不必回收原数组空间, 只需要让numFilled=0, 用新值覆盖旧值就可以了;
 */
void PQHeap::clear() {
    this->numFilled = 0;
}

/*
 * 打印出数组形式的堆
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << this->elements[i] << endl;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (this->numFilled > this->numAllocated)
        error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare priority of
     * neighboring elements. If current element has larger priority
     * than the previous, array elements are out of order with respect
     * to our intention. Use error to report this problem.
     */
    int numElements = size();
    for(int i=0; i<numElements; ++i) {
        int left = 2*i + 1;
        int right = left + 1;
        if(left < numElements) {  // 此处一定小于, 7个元素下标最大为6
            if(this->elements[i].priority > this->elements[left].priority) {
                printDebugInfo("validateInternalState");
                error("PQArray has elements out of order at index " + integerToString(i));
            }
        }
        if(right < numElements) {  // 此处一定小于, 7个元素下标最大为6
            if(this->elements[i].priority > this->elements[right].priority) {
                printDebugInfo("validateInternalState");
                error("PQArray has elements out of order at index " + integerToString(i));
            }
        }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    int parent = (child - 1) / 2;
    if(parent >= 0)
        return parent;
    else
        return NONE;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int leftChild = 2 * parent + 1;
    if(leftChild < size())  // 必须小于, 7个元素下标最大为6
        return leftChild;
    else
        return NONE;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int rightChild = 2 * parent + 2;
    if(rightChild < size())  // 必须小于, 7个元素下标最大为6
        return rightChild;
    else
        return NONE;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */
STUDENT_TEST("测试入队出队") {
    PQHeap pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

STUDENT_TEST("在每一次入队都进行中间状态检测") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.validateInternalState();
    pq.enqueue({ "pi", 3.14 });
    pq.validateInternalState();
    pq.enqueue({ "phi", 1.618 });
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

STUDENT_TEST("测试size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("测试空队列出队入队报错") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

STUDENT_TEST("在每一次出队都进行中间状态检测") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.enqueue({ "pi", 3.14 });
    pq.enqueue({ "phi", 1.618 });

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

STUDENT_TEST("测试扩充数组分配内存") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

STUDENT_TEST("测试一系列混合操作") {
    PQHeap pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

STUDENT_TEST("压力测试, 循环入队出队随机数") {
    PQHeap pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

STUDENT_TEST("测试相同优先级元素输出") {
    for (int size = 5; size <= 8; size *= 5) {
        PQHeap pq;
        // 先入队多个-3优先级, 最后入队-4
        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", -3 });
        }
        pq.enqueue({"", -4 });
        pq.validateInternalState();
        DataPoint expected = {"", -4 };
        EXPECT_EQUAL(pq.dequeue(), expected);

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", -3 };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

STUDENT_TEST("测试相同优先级元素输出V2") {
    for (int size = 5; size <= 8; size *= 5) {
        PQHeap pq;
        // 先入队-2, 最后入队多个-3优先级
        pq.enqueue({"", -2 });
        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", -3 });
        }
        pq.validateInternalState();
        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", -3 };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
        DataPoint expected = {"", -2 };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

STUDENT_TEST("时间复杂度测试") {
    PQHeap pq;
    int n = 20000;
    for(int i=0; i<4; ++i) {
        TIME_OPERATION(n, fillQueue(pq, n));
        TIME_OPERATION(n, emptyQueue(pq, n));
        n *= 2;
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
