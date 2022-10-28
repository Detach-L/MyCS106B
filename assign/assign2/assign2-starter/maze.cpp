/*
 * 本文件使用队列和栈实现寻找迷宫路径算法.
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/*
 * 本函数用来判断一个给定位置是否超出了迷宫边界;
 * Grid本身有inBounds函数, 所以不必重复写此函数;
 */
//bool isInMazeBound(const Grid<bool>& maze,const GridLocation& location) {
//    if(location.row < 0 || location.col < 0)
//        return false;
//    if(location.row > maze.numRows() - 1 || location.col > maze.numCols() - 1)
//        return false;  // maze.numRows()需要减1, 这里是坐标比较
//    return true;
//}

/*
 * 本函数用来找到给定迷宫位置的所有有效相邻位置,
 * 有效表示该位置需要满足三个条件:
 *     1. 只能移动一步, 且只能沿上下左右四个方向移动;
 *     2. 只能在迷宫所在的区域内移动，不能越界;
 *     3. 只能沿着通道移动，不能穿过迷宫墙壁;
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    // 上 下 左 右
    Vector<GridLocation> moves;
    moves.add({-1, 0});  // 上
    moves.add({1, 0});  // 下
    moves.add({0, -1});  // 左
    moves.add({0, 1});  // 右
    for(int i=0; i < moves.size(); i++) {
        GridLocation moveCur = {cur.row + moves[i].row, cur.col + moves[i].col};
//        if(isInMazeBound(maze, moveCur) && maze[moveCur])
        if(maze.inBounds(moveCur) && maze[moveCur])
            neighbors.add(moveCur);
    }
    return neighbors;
}

/*
 * 本函数用来判断一条给定路径是否是迷宫的一个可行解;
 * 可行解需满足以下条件:
 *      1. 路径起始点在迷宫左上角, 路径终点在迷宫右下角;
 *      2. 从当前位置移动到下一位置时，每一步都必须是符合规则的移动;
 *      3. 路径中不能出现循环，即同一个位置不能走两次，每次都要走没走过的路;
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntrance = {0,  0};

    if (path.peek() != mazeExit) {  // 判断路径终点是否合法
        error("Path does not end at maze exit");
    }

    Set<GridLocation> visitedLoc;
    while(!path.isEmpty()) {
        GridLocation curLoc = path.pop();

        if(visitedLoc.contains(curLoc))  // 同一个位置不能走两次
            error("Path can't contain loop");
        visitedLoc.add(curLoc);

        if(path.isEmpty()) {  // 循环里, stack最后一个值path.pop()后变成空
            if (curLoc != mazeEntrance) {  // 判断路径起点是否合法
                error("Path does not start at maze entrance");
            }
            break; // 为空就不用判断上一步路径的移动是否合法了
        }

        Set<GridLocation> validMoves = generateValidMoves(maze, path.peek());
        if(!validMoves.contains(curLoc))  // 上一步路径的合法移动没有包含当前路径
            error("Path movement is invalid");
    }
}

/**
 * @brief visitedLocation - 由于栈无法遍历查询，转换为集合类型
 * @param curPath
 * @return 以集合的形式返回栈中包含的所有坐标
 */
Set<GridLocation> visitedLocationSet(Stack<GridLocation> curPath) {
    Set<GridLocation> gl;
    while(!curPath.isEmpty()){
        gl.add(curPath.pop());
    }
    return gl;
}

/*
 * 通过队列和栈, 找到给定迷宫的最短路径;
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    path.push({0, 0});

    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    Queue<Stack<GridLocation>> toDo;
    toDo.enqueue(path);
    while(!toDo.isEmpty()) {
        Stack<GridLocation> curPath = toDo.dequeue();
        if(curPath.peek() == mazeExit) {  // 终点, 找到一条路径
            MazeGraphics::highlightPath(curPath, "green", 4);
            return curPath;
        }

        Set<GridLocation> neibors = generateValidMoves(maze, curPath.peek());
        Set<GridLocation> visitedLocation = visitedLocationSet(curPath);
        for(const GridLocation& neibor: neibors){  // 将当前位置所有下一步路径加入队列
            if(!visitedLocation.contains(neibor)) {  // 不能有循环
                Stack<GridLocation> moveOneStepPath = curPath;
                moveOneStepPath.push(neibor);
//                visitedLocation.add(neibor);  // 思考下这一句有必要加吗?
                // 不用加, 已经考虑不回头, 每次走上下左右一步, neibor不会重复
                toDo.enqueue(moveOneStepPath);
                MazeGraphics::highlightPath(moveOneStepPath, "green", 4);
            }
        }
    }

    return {};
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };  // 斜着走
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

/* * * * * * Student Test Cases * * * * * */
/* * * generateValidMoves 验证 */
// 验证是否能返回空值
STUDENT_TEST("generateValidMoves on location in the center of 1x1 grid with no walls") {
    Grid<bool> maze = {{true}};
    GridLocation center = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

// 验证左下角位置
STUDENT_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {maze.numRows() - 1, 0};  // {2,0} --> {1,0} {2,1}
    Set<GridLocation> expected = {{1, 0}, {maze.numRows() - 1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

// 验证右下角位置
STUDENT_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {maze.numRows() - 1, maze.numCols() - 1};  // {2,2} --> {2,1} {1,2}
    Set<GridLocation> expected = {{maze.numRows() - 2, maze.numCols() - 1}, {maze.numRows() - 1, maze.numCols() - 2}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

// 验证有墙壁情况下左下角位置
STUDENT_TEST("generateValidMoves on location on the side of 3x3 grid with walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, false},
                       {true, true, true}};
    GridLocation side = {maze.numRows() - 1, maze.numCols() - 1};  // {2,2} --> {2,1} {1,2}
    Set<GridLocation> expected = {{maze.numRows() - 1, maze.numCols() - 2}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}
/**/

/* * * validatePath 验证 */
STUDENT_TEST("validatePath on multiple correct solutions") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, false},
                       {true, true, true}};
    Stack<GridLocation> soln1 = { {0 ,0}, {0, 1}, {1, 1}, {2, 1}, {2, 2} };
    Stack<GridLocation> soln2 = { {0 ,0}, {1, 0}, {1, 1}, {2, 1}, {2, 2} };
    Stack<GridLocation> soln3 = { {0 ,0}, {1, 0}, {2, 0}, {2, 1}, {2, 2} };
    Stack<GridLocation> soln4 = { {0 ,0}, {0, 1}, {1, 1}, {1, 0}, {2, 0}, {2, 1}, {2, 2} };

    EXPECT_NO_ERROR(validatePath(maze, soln1));
    EXPECT_NO_ERROR(validatePath(maze, soln2));
    EXPECT_NO_ERROR(validatePath(maze, soln3));
    EXPECT_NO_ERROR(validatePath(maze, soln4));
}

STUDENT_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/19x35.maze", maze);
    readSolutionFile("res/19x35.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, true, true},
                       {false, true, false},
                       {true, true, true}};
    // 入口出口条件
    Stack<GridLocation> not_end_at_exit = { {0, 0}, {2, 1} };
    Stack<GridLocation> not_begin_at_entry = { {0, 1}, {1, 1}, {2, 1}, {2, 2} };

    // 合法移动条件
    Stack<GridLocation> go_through_wall = { {0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2} };
    Stack<GridLocation> teleport = { {0, 0}, {1, 0}, {1, 1}, {2, 2} };  // 斜着走
    Stack<GridLocation> go_through_two_step = { {0, 0}, {1, 0}, {2, 1}, {2, 2} };
    Stack<GridLocation> go_out_of_maze_1 = { {0, 0}, {3, 2}, {2, 2} };
    Stack<GridLocation> go_out_of_maze_2 = { {0, 0}, {2, 3}, {2, 2} };
    Stack<GridLocation> go_out_of_maze_3 = { {0, 0}, {2, -1}, {2, 0}, {2, 1}, {2, 2} };
    Stack<GridLocation> go_out_of_maze_4 = { {0, 0}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {2, 2} };

    // 循环路径
    Stack<GridLocation> revisit = { {0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 0}, {2, 1}, {2, 2} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, go_through_two_step));
    EXPECT_ERROR(validatePath(maze, go_out_of_maze_1));
    EXPECT_ERROR(validatePath(maze, go_out_of_maze_2));
    EXPECT_ERROR(validatePath(maze, go_out_of_maze_3));
    EXPECT_ERROR(validatePath(maze, go_out_of_maze_4));
    EXPECT_ERROR(validatePath(maze, revisit));
}
/**/

/* * * solveMaze 验证 */
STUDENT_TEST("solveMaze on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
/**/
