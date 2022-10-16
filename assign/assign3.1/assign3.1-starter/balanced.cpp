/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * 使用递归从字符串中提取大中小括号
 */
string operatorsFrom(string str) {
    // edge case
    if(str.empty())
        return "";
    // handle first character and merge rest
    string filterStr = "{}[]()";
    if(filterStr.find(str[0]) != string::npos)
        return str[0] + operatorsFrom(str.substr(1));
    else
        return operatorsFrom(str.substr(1));
}

/* * * * * * STUDENT Test Cases * * * * * */

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("(([a])"), "(([])");
    EXPECT_EQUAL(operatorsFrom("3)("), ")(");
    EXPECT_EQUAL(operatorsFrom("{(x}y)"), "{(})");
    EXPECT_EQUAL(operatorsFrom("int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }"), "(){([])(())}");
}

/*
 * 检查输入的仅含大中小括号的字符串是否匹配
 */
bool operatorsAreMatched(string ops) {
    // edge case
    if(ops.empty())
        return true;
    // find the first matching pair of operators and remove them from ops
    if(ops.find("()") != string::npos)
        ops.erase(ops.find("()"), 2);
    else if(ops.find("[]") != string::npos)
        ops.erase(ops.find("[]"), 2);
    else if(ops.find("{}") != string::npos)
        ops.erase(ops.find("{}"), 2);
    else  // 配对括号找不到但字符串不为空
        return false;
    // check rest
    return operatorsAreMatched(ops);
}

/* * * * * * STUDENT Test Cases * * * * * */

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT(operatorsAreMatched("{()[]}"));
    EXPECT(operatorsAreMatched("{([()])}"));
    EXPECT(operatorsAreMatched("(()){}{[{()}]}"));
    EXPECT(!operatorsAreMatched("{([)]}"));
    EXPECT(!operatorsAreMatched(")"));
    EXPECT(!operatorsAreMatched("{["));
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
