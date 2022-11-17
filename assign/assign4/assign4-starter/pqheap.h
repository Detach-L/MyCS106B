#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/**
 * Priority queue of DataPoints implemented using a binary heap.
 */
class PQHeap {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQHeap();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQHeap();

    /**
     * Adds a new element into the queue. This operation runs in time O(log n),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in this priority queue.
     * The frontmost element is the one with the most urgent priority. A priority
     * of 1 is more urgent than priority 2 which is more urgent than priority 7
     * and so on. If the priority queue contains two or more elements of equal
     * priority, the order those elements are dequeued is arbitrary, i.e. there
     * is no required tie-break handling.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(log n).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether this priority queue is empty.
     *
     * This operation must run in time O(1).
     *
     * @return true if contains no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the count of elements in this priority queue.
     *
     * This operation must run in time O(1).
     *
     * @return The count of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation must run in time O(1).
     */
    void clear();

    /*
     * This function exists purely for testing purposes. You can have it do whatever you'd
     * like and we won't be invoking it when grading. In the past, students have had this
     * function print out the array representing the heap, or information about how much
     * space is allocated, etc. Feel free to use it as you see fit!
     */
    void printDebugInfo(std::string msg) const;

    /*
     * This function exits purely for testing purposes. It verifies
     * that the internal state of the queue is valid/consistent.
     * If a problem is detected, this function calls error().
     * If no problem, the function returns normally.
     */
    void validateInternalState() const;

private:
    DataPoint* elements;  // dynamic array to save heap elements
    int numAllocated;  // number of slots allocated in array
    int numFilled;  // number of slots filled in array

    /* While not a strict requirement, we strongly recommend implementing the
     * helper functions declared below. They will make your code much cleaner, and
     * will aid in the debuggin process.
     */
    int getParentIndex(int child) const;
    int getLeftChildIndex(int parent) const;
    int getRightChildIndex(int parent) const;

    /*
     * 判断队列分配内存是否已用完;
     * 用完则返回true, 否则返回false
     * O(1)
     */
    bool isFull() const;

    /*
     * 队列分配内存已用完时扩容, multiple表示扩容倍数;
     * 扩容需要先移动当前元素到新队列, 然后删除旧队列;
     * O(n)
     */
    void expand(int multiple);

    /*
     * 以二叉堆存储队列, 若优先级值越小表示越紧急时, 则堆顶元素优先级值最小;
     *     1.若队列已满, 需要扩容
     *     2.若队列不满:
     *         2.1将元素插入二叉堆尾部
     *         2.2若插入节点比父节点小, 则交换两节点位置, 然后重复2.2;
     *            若插入节点比父节点大, 则退出;
     * O(logn)
     */
    void insertToArray(const DataPoint& elem);

    /*
     * 以二叉堆存储队列, 若优先级值越小表示越紧急时, 则堆顶元素优先级值最小;
     * 删除堆顶元素, 也即数组下标0元素;
     * 重建二叉堆:
     *     1.将堆中最后一个元素移动到堆顶
     *     2若堆顶元素比其左右子结点最小的那个大, 则交换两节点位置, 然后重复2;
     *      否则退出;
     * O(logn)
     */
    void deleteFromArray();

    void swap(DataPoint& indexA, DataPoint& indexB);
    int getMinChildIndex(int indexLeft, int indexRight);

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQHeap);
};
