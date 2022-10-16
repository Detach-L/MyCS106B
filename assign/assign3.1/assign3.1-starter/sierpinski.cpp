/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * 使用递归的方式, 绘制阶数为n的Sierpinski三角形, 并且返回绘制三角形个数
 * Sierpinski三角形:
 *      Order-n阶的Sierpinski三角形是由3个Order-(n-1)阶的Sierpinski三角形组成,
 *      它们的边长是原始边长的一半, 彼此之间顶点相连排列;
 * 绘制2阶的, 就相当于绘制三个1阶三角形;
 * 而绘制1阶的, 就相当于绘制三个0阶三角形;
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    // edge case, 内部不再绘制, 返回一个三角形
    if(order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    int count = 0;
    // first triangle, 以one为其中一个顶点
    count += drawSierpinskiTriangle(window,
                           one,
                           GPoint((one.x+two.x)/2, (one.y+two.y)/2),
                           GPoint((one.x+three.x)/2, (one.y+three.y)/2),
                           order-1);
    // second triangle, 以two为其中一个顶点
    count += drawSierpinskiTriangle(window,
                           two,
                           GPoint((two.x+one.x)/2, (two.y+one.y)/2),
                           GPoint((two.x+three.x)/2, (two.y+three.y)/2),
                           order-1);
    // third triangle, 以three为其中一个顶点
    count += drawSierpinskiTriangle(window,
                           three,
                           GPoint((three.x+two.x)/2, (three.y+two.y)/2),
                           GPoint((three.x+one.x)/2, (three.y+one.y)/2),
                           order-1);
    return count;
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

