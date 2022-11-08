/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * 计算一个单词得分
 * 得分规则：
 *    具有 3 个或更少字符的单词无效，因此记零分。
 *    一个 4 个字母的单词获得 1 分；
 *    一个 5 个字母的单词获得 2 分；
 *    6 个字母的单词获得 3 分；以此类推
 */
int points(string str) {
    return (str.length() >= 3) ? (str.length() - 3) : 0;
}

/*
 * 生成向四个基线方向和四个对角线方向的移动
 * [
 *  {-1,-1}, {-1,0}, {-1,1},
 *  {0,-1},  {0,1},
 *  {1,-1}, {1,0}, {1,1},
 * ]
 */
static Vector<GridLocation>& getValidMoves(int center=1) {
    static Vector<GridLocation> moves;
    int row = 2*center + 1;
    int col = 2*center + 1;
    for(int i=0; i<row; ++i) {
        for(int j=0; j<col; ++j) {
            if(i==center && j==center)  // 去除中间没移动的那步
                continue;
            moves.add({i-center, j-center});
        }
    }
    return moves;
}

/*
 * 生成给定方格中的某个位置的所有有效邻居点
 * 不越界, 有效邻居点为四个基线方向和四个对角线方向所有1范围方格
 */
Set<GridLocation> generateValidNeighbors(const Grid<char>& board,
                                         const Vector<GridLocation>& moves,
                                         const GridLocation& cur) {
    Set<GridLocation> neighbors;
    for(int i=0; i < moves.size(); i++) {
        GridLocation moveCur = {cur.row + moves[i].row, cur.col + moves[i].col};
        if(board.inBounds(moveCur))
            neighbors.add(moveCur);
    }
    return neighbors;
}

/*
 * 在v1和v2版本用此函数将visited数组转成集合, 以判断是否走了回头路
 */
Set<GridLocation> vecToSet(const Vector<GridLocation>& visited){
    Set<GridLocation> s;
    for(const auto& loc: visited){
        s.add(loc);
    }
    return s;
}

#define v3
#ifdef v3
/*
 * 拼字游戏 --- 递归回溯方式实现
 * 目标：找到拼字盘上所有单词并达到尽可能高的分数
 * 相较于V2, 不再用visited Vector判断是否包含已访问方格, 而是用Grid<bool>& visitedBoard
 */
void scoreBoardRec(Grid<char>& board,
                  Lexicon& lex,
                  Vector<GridLocation>& moves,
                  Grid<bool>& visitedBoard,
                  const GridLocation& curLoc,
                  string& curWord,
                  Set<string>& words,
                  int& tolCount) {
    if (curWord.length() > 3 && lex.contains(curWord)) {  // 小于3不能直接返回, 可能还有更加长的单词
        if(!words.contains(curWord)) {  // 防止重复单词
            words.add(curWord);
            tolCount += points(curWord);
        }
    }
    Set<GridLocation> neighbors = generateValidNeighbors(board, moves, curLoc);

    for(const auto neighbor: neighbors) {
        if(!visitedBoard.get(neighbor)) {
            visitedBoard.set(neighbor, true);
            curWord += board.get(neighbor);
            if(lex.containsPrefix(curWord)) { // 不包含此前缀跳出循环遍历下一个
                scoreBoardRec(board, lex, moves, visitedBoard, neighbor, curWord, words, tolCount);
            }
            visitedBoard.set(neighbor, false);  // 恢复状态
            curWord.erase(curWord.size() - 1);
        }
    }
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /*
     * 大体思路：
     *      遍历方格中每个格子, 每个格子都递归地计算得分
     *      同时用一个与方格相同大小的grid记录访问过的方格 || 或者将Vector变成set, 用contains判断
     *          在递归里, 每一次都遍历邻居点, DFS探索, 探索边界是某个方格没有邻居点
     *          剪枝, 当单词表里没有以这个为前缀的单词, 则停止探索此点
     */
    int tolCount = 0;  // 记录得分
    Set<string> words;  // 记录所有单词
    Grid<bool> visitedBoard(board.numRows(), board.numCols(), false);  // O(N)
    auto moves = getValidMoves();  // 获取每个点可能移动步
    string curWord;
    GridLocation curLoc;
    // 遍历所有方格
    for(int row=0; row<board.numRows(); ++row) {
        for(int col=0; col<board.numCols(); ++col) {
            curLoc = {row, col};
            visitedBoard.set(curLoc, true);
            curWord += board.get(curLoc);
            if(lex.containsPrefix(curWord)) {
                scoreBoardRec(board, lex, moves, visitedBoard, curLoc, curWord, words, tolCount);
            }
            visitedBoard.set(curLoc, false);  // 恢复状态
            curWord.erase(curWord.size() - 1);
        }
    }
    return tolCount;
}
#endif

#ifdef v2
/*
 * 拼字游戏 --- 递归回溯方式实现
 * 目标：找到拼字盘上所有单词并达到尽可能高的分数
 * 相较于V1将auto visitedSet = vecToSet(visited);放到了循环外
 */
void scoreBoardRec(Grid<char>& board,
                  Lexicon& lex,
                  Vector<GridLocation>& moves,
                  Vector<GridLocation>& visited,
                  string& curWord,
                  Set<string>& words,
                  int& tolCount) {
    if (curWord.length() > 3 && lex.contains(curWord)) {  // 小于3不能直接返回, 可能还有更加长的单词
        if(!words.contains(curWord)) {  // 防止重复单词
            words.add(curWord);
            tolCount += points(curWord);
        }
    }
    GridLocation curLoc = visited.get(visited.size() - 1);  // 当前方格
    Set<GridLocation> neighbors = generateValidNeighbors(board, moves, curLoc);
    auto visitedSet = vecToSet(visited);
    for(const auto neighbor: neighbors) {
        if(!visitedSet.contains(neighbor)) {
            visited.add(neighbor);
            //visitedSet.add(neighbor);  // for循环遍历, 不会重复走邻居点, 所以不用更新visitedSet
            curWord += board.get(neighbor);
            if(lex.containsPrefix(curWord)) { // 不包含此前缀跳出循环遍历下一个
                scoreBoardRec(board, lex, moves, visited, curWord, words, tolCount);
            }
            visited.remove(visited.size() - 1);  // 恢复状态
            //visitedSet.remove(neighbor);
            curWord.erase(curWord.size() - 1);
        }
    }
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /*
     * 大体思路：
     *      遍历方格中每个格子, 每个格子都递归地计算得分
     *      同时用一个与方格相同大小的grid记录访问过的方格 || 或者将Vector变成set, 用contains判断
     *          在递归里, 每一次都遍历邻居点, DFS探索, 边界是某个方格没有邻居点
     *          剪枝, 当单词表里没有以这个为前缀的单词, 则停止探索此点
     */
    int tolCount = 0;  // 记录得分
    Set<string> words;  // 记录所有单词
    Vector<GridLocation> visited;
    auto moves = getValidMoves();  // 获取每个点可能移动步
    // 遍历所有方格
    for(int row=0; row<board.numRows(); ++row) {
        for(int col=0; col<board.numCols(); ++col) {
            visited.clear();  // 每个方格重置visit  O(1)
            visited.add({row, col});  // O(1)
            string curWord;
            curWord += board.get({row, col});
            if(lex.containsPrefix(curWord)) {
                scoreBoardRec(board, lex, moves, visited, curWord, words, tolCount);
           }
        }
    }
    return tolCount;
}
#endif

#ifdef v1
/*
 * 拼字游戏 --- 递归回溯方式实现
 * 目标：找到拼字盘上所有单词并达到尽可能高的分数
 * 游戏规则：
 *  - 单词必须至少有 4 个字母
 *  - 单词必须包含在有效的英语单词列表lex中
 *  - 单词可以通过跟踪相邻的字母立方体路径来形成，最多使用一次立方体
 *    (沿着四个基线方向和四个对角线方向彼此相邻, 每个立方体有3到8个邻居)
 *  - 单词不能重复记分，每个单词只记分一次
 * 提示：
 *  - 回溯
 *      一般的想法是从 16 个字母立方体中的每个立方体开始进行搜索，
 *      递归探索从起点开始的所有路径，并跟踪其相邻立方体。
 *  - 标记
 *      需要某种方法来“标记”是否访问过字母立方体
 *  - 精简
 *      Lexicon中的containsPrefix函数会判断单词是否以给定的前缀开头。
 *      如果你检查的第一个立方体是字母Z，其邻居是 X，则初始路径为 ZX。
 *      函数lex.containsPrefix("ZX")的调用将返回false，通知你词典中不包含以此前缀开头的单词。
 *      搜索应该放弃这条路径，转向其他更有希望的组合。
 */
void scoreBoardRec(Grid<char>& board,
                  Lexicon& lex,
                  Vector<GridLocation>& moves,
                  Vector<GridLocation>& visited,
                  string& curWord,
                  Set<string>& words,
                  int& tolCount) {
    // base case
//    if(!lex.containsPrefix(curWord)) {  // 不包含此前缀直接返回
//        return;  // ======== 不能在此处判断, 一开始curWord为空, 也是没有的, 直接返回了 ======== //
//    }else
    if (curWord.length() > 3 && lex.contains(curWord)) {  // 小于3不能直接返回, 可能还有更加长的单词
        if(!words.contains(curWord)) {  // 防止重复单词
            words.add(curWord);
            tolCount += points(curWord);
        }
    }
    // 现在还有两个问题：1. 不走回头路  2. 从上一次探索恢复状态
    GridLocation curLoc = visited.get(visited.size() - 1);
//    char curChar = board.get(curLoc);  // 此变量没啥用了
    Set<GridLocation> neighbors = generateValidNeighbors(board, moves, curLoc);
    for(const auto neighbor: neighbors) {
        auto visitedSet = vecToSet(visited);  // 感觉每个循环都要这样效率不高, 可以放到外面, 也是每次探索后恢复状态
        if(!visitedSet.contains(neighbor)) {
            visited.add(neighbor);
            curWord += board.get(neighbor);
            if(lex.containsPrefix(curWord)) { // 不包含此前缀直接返回, 此处不能直接返回, 会结束函数而不是跳出循环遍历下一个
                scoreBoardRec(board, lex, moves, visited, curWord, words, tolCount);
            }
            visited.remove(visited.size() - 1);  // 恢复状态
            curWord.erase(curWord.size() - 1);
        }
    }
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /*
     * 大体思路：
     *      遍历方格中每个格子, 每个格子都递归地计算得分
     *      同时用一个与方格相同大小的grid记录访问过的方格 || 或者将Vector变成set, 用contains判断
     *          在递归里, 每一次都遍历邻居点, DFS探索, 边界是某个方格没有邻居点
     *          剪枝, 当单词表里没有以这个为前缀的单词, 则停止探索此点
     */
    int tolCount = 0;
    Set<string> words;
    Vector<GridLocation> visited;  // O(N)
    auto moves = getValidMoves();
    // Traverse all location
    for(int row=0; row<board.numRows(); ++row) {
        for(int col=0; col<board.numCols(); ++col) {
            visited.clear();  // 每个方格重置visit  O(1)
            visited.add({row, col});  // O(1)
            string curWord;
            curWord += board.get({row, col});
            if(lex.containsPrefix(curWord)) {
                // 不能用scoreBoardRec递归返回值记得分, 因为小于等于3的单词没有分数, 得用引用
                scoreBoardRec(board, lex, moves, visited, curWord, words, tolCount);
           }
        }
    }
    return tolCount;
}
#endif

static Lexicon& sharedLexicon();

//STUDENT_TEST("Test scoreBoard, board contains one word, score of 1") {
//    Grid<char> board = {{'C','_','_','_'},
//                        {'Z','_','_','_'},
//                        {'_','A','_','_'},
//                        {'_','_','R','_'}};
//    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
//}


//STUDENT_TEST("Test scoreBoard, board contains one word, score of 1") {
//    Grid<char> board = {{'L','I','_','_'},
//                        {'M','E','_','_'},
//                        {'_','S','_','_'},
//                        {'_','_','_','_'}};
//    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};
//    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
//}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
