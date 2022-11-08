// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "hashmap.h"
#include "error.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * 输入一个元素为int类型列表, 返回其所有元素和
 */
int sumList(Vector<int>& elems) {
    int sum = 0;
    for(const auto& elem: elems)
        sum += elem;
    return sum;
}

/*
 * 根据每个区块的关键选票出现的次数, 将其转换成Banzhaf 影响力指数
 */
void votesCountToBanzhaf(Vector<int>& result) {
    int totalVotes = sumList(result);
    if(totalVotes == 0)
        error("没有关键选区!");
    for(auto& elem: result) {
        elem = elem * 100 / totalVotes;
    }
}

#define v2

#ifdef v2
/*
 * 此版本相较于v1, 不在寻找获胜联盟, 选择一个区块, 看他是否是某联盟的关键选区
 *
 * 整体思路: 先写出生成区块所有子集的递归代码, 然后考虑优化以及剪枝(约束条件判断)
 * 输入某个区块选票数, 输出区块影响力
 * 优化:
 *      不需要先找到获胜的联盟，然后再确定哪些参与者是关键的;
 *      可以持续计算特定于某个目标区块的关键选票，并对所有区块重复该过程;
 *      也即, 选择某个目标区块, 递归地探索剩余区块组成的的子集(联盟),
 *      一旦得到一个子集形成一个联盟(每次递归都累计当前子集的票数),
 *      就判断如果没有目标区块，联盟是否会失败, 有目标区块，联盟是否会赢,
 *      满足没有会失败且有会赢则目标区块是关键选区;
 *      递归判断其他选区;
 *      优点:
 *          这样的方法把先找到获胜的联盟，然后再确定哪些参与者是关键的这样的搜索过程融合到递归里了;
 *          更方便减少重复票数选区的计算;
 */
void computePowerIndexesRec(Vector<int>& blocks,
                                   int& halfVotes,
                                   int& curTotalVotes,
                                   int& keyCounts,
                                   int selected,
                                   int index){
    for(int i=index; i<blocks.size(); ++i){
        if(i != selected) {  // 探索不包含当前选择的目标选区
            curTotalVotes += blocks[i];  // 求和当前选票总数
            // 获胜联盟, 总票数大于半数(算半数的话此处是严格大于而不是大于等于, 后面是小于等于)
            if(curTotalVotes <= halfVotes) {  // 只有当前不是获胜联盟, 才有必要探索加不加目标选区, 以及以此为基础构建的联盟
                if( curTotalVotes + blocks[selected] > halfVotes)  // 加上目标选区, 获胜联盟, 则关键选区;
                    keyCounts++;
                // 探索生成其他集合  // 注意此处是在i基础上探索
                computePowerIndexesRec(blocks, halfVotes, curTotalVotes, keyCounts, selected, i+1);
            }
            curTotalVotes -= blocks[i];  // 探索完恢复状态
        }
    }
}

/*
 * 递归实现寻找关键选区 --- Include/Exclude模式
 */
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    HashMap<int, int> resultMemory;  // 存下探索过的票数
    int halfVotes = sumList(blocks) / 2;  // 计算想要获胜的票数(半数票票数)
    for(int i=0; i<blocks.size(); ++i) {
        if(!resultMemory.containsKey(blocks[i])) {  // 如果之前没计算过该值区块
            int keyCounts = 0;  // 选择区块关键选区计数
            int curTotalVotes = 0;  // 当前集合票数总和
            computePowerIndexesRec(blocks, halfVotes, curTotalVotes, keyCounts, i, 0);
            result.add(keyCounts);
            resultMemory.put(blocks[i], keyCounts);
        } else {  // 重复区块不用计算了
            result.add(resultMemory.get(blocks[i]));
        }
    }
    votesCountToBanzhaf(result);
    return result;
}
#endif

#ifdef v1
/*
 * 此版本是递归找到所有子集, 如果当前子集是获胜联盟, 然后判断哪些参与者是关键的
 *
 * 整体思路: 先写出生成区块所有子集的递归代码, 然后考虑优化以及剪枝(约束条件判断)
 * 输入某个区块选票数, 输出区块影响力
 * 优化: 从作业注意事项里看看如何优化
 *      1. 如果{50,49}已经是获胜联盟了, 那{50,49,1}还需要判断?需要, 50在这里面还是关键选区
 *      2. 对于重复值, 不必重复计算, 怎么优化?还是把votes值当作key, 判断HashMap是否有这个key了?
 */
void computePowerIndexesRec(Vector<int>& blocks,
                                   Vector<int>& result,
                                   int& halfVotes,
                                   int& curTotalVotes,
                                   HashMap<int, int>& soFar,  // 下标到值的映射(因为下标唯一, 适合当key, 不然用值当key测试二有重复值过不了)
                                   int index){
    for(int i=index; i<blocks.size(); ++i){
        soFar.put(i, blocks[i]);  // 选择一个探索
        curTotalVotes += blocks[i];  // 求和当前选票总数

        if(curTotalVotes > halfVotes) { // 获胜联盟, 总票数大于半数(算半数的话此处是严格大于而不是大于等于)
            for(const auto& key: soFar) { // 去除该集合某元素, 看总票数与绝对数大小, 以得到关键选区
                if(curTotalVotes - soFar.get(key) <= halfVotes) {  // 上面是>, 此处就是<=
                    result[key]++;
                }
            }
        }
        // 探索生成其他集合
        computePowerIndexesRec(blocks, result, halfVotes, curTotalVotes, soFar, i+1);  // 注意此处是在i基础上探索

        soFar.remove(i);  // 探索完恢复状态
        curTotalVotes -= blocks[i];  // 探索完恢复状态
    }
}

/*
 * 递归实现寻找关键选区 --- Include/Exclude模式
 */
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    for(int i=0; i<blocks.size(); ++i) {
        result.add(0);
    }
    HashMap<int, int> soFar;
    int halfVotes = 0;  // 计算想要获胜的绝对票数
    int curTotalVotes = 0;  // 当前集合票数总和
    halfVotes = sumList(blocks) / 2;  // 计算半数票票数
    computePowerIndexesRec(blocks, result, halfVotes, curTotalVotes, soFar, 0);
    votesCountToBanzhaf(result);
    return result;
}
#endif

/* * * * * * Test Cases * * * * * */
STUDENT_TEST("Time power index operation") {
    Vector<int> blocks;
    int k = 3;
    for (int j = 0; j < 4; j++) {
        blocks.clear();
        for (int i = 0; i < k; i++) {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
        k *= 2;
    }
}

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


