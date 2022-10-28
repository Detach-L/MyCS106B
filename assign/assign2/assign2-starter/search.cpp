// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "simpio.h"
using namespace std;


/*
 * 本函数对输入字符串做下述标准化处理:
 *   1. 清除字符串首尾符号, 保留内部字符;
 *      再写buildIndex时发现对第一点理解有问题,
 *      <section>和<<section>>都返回section
 *   2. 确保字符串至少含有一个字符, 否则返回""
 *   3. 将字符串字符转换为小写
 */
string cleanToken(string s)
{
    if(s.size() == 0)
        return "";
    bool emptyFlag = true;  // 此flag作用是将类似"106"这样的数字串返回""

    for(int i=0; i < s.size(); i++){  // 把每个字符改成小写
        if(isalpha(s.at(i))){
//            s.at(i) = toLowerCase(s.at(i));
            if(emptyFlag) {  // 有字符就把emptyFlag置为false
                emptyFlag = false;
                break;
            }
        }
    }
    if(emptyFlag)  // 都是符号返回空
        return "";

    s = toLowerCase(s);  // toLowerCase可以直接操作字符串

    int startIndex = 0;  // 没必要每次都substr, 而是用index记录操作
    int endIndex = 0;  // endIndex不能等于s.size()然后while里endIndex--, substr取的是字符个数, 会有问题
    while(ispunct(s.at(startIndex))) {  // 清除首尾符号, if改成while
//        s = s.substr(1);
        startIndex++;
    }
    while(ispunct(s.at(s.size() - endIndex - 1))) {  // 此处是下标, 得减一
//        s = s.substr(0, s.size() - 1);
        endIndex++;
    }
    return s.substr(startIndex, s.size() - endIndex - startIndex);  // 减去要删除字符个数
}

/*
 * 本函数收集字符串中的所有无重复单词;
 */
Set<string> gatherTokens(string text)
{
    Set<string> tokens;

    Vector<string> wordList = stringSplit(text, " ");
    for(string& word: wordList) {
        word = cleanToken(word);
        if(word.size() > 0)  // 提供测试三, ""字符串不应该加入token
            tokens.add(word);
    }
    return tokens;
}

/*
 * 读取网页数据库文件, 构建反向索引;
 * 数据库文件格式: 一行url一行文本交替;
 * 反向索引: 单词到url的映射, 如果某个url的文本包含该单词, 则构建一个反向索引;
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;
    if(!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);

    int urlCount = 0;
    for(int i=0; i < lines.size(); i+=2) {
        string url = lines.get(i);
        string content = lines.get(i+1);
        if(content.size() > 0 && url.size() > 0){
            Set<string> tempTokens = gatherTokens(content);
            for(const string& tempToken: tempTokens){
                index[tempToken].add(url);
            }
            urlCount++;
        }
    }
    return urlCount;
}

/*
 * 根据查询指令以及反向索引, 返回相应的网页内容;
 * 查询指令: 类似tasty -mushrooms simple +cheap,
 *     1. 以空格分开每一条指令, 从左到右获取每一个指令结果
 *     2. 指令前没有+或-表示左边指令对右边指令结果求并集
 *     3. 其中指令前面的+表示左边指令对右边指令结果求交集
 *     4. 其中指令前面的-表示左边指令对右边指令结果求差集
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;

    Vector<string> queryKeys = stringSplit(query, " ");
    string firstKey = cleanToken(queryKeys.get(0));
    result = index[firstKey];

    for(int i=1; i < queryKeys.size(); i++) {
        string queryKey = queryKeys.get(i);
        char queryType = queryKey.at(0);
        queryKey = cleanToken(queryKey);
        Set<string> tmpResult = index[queryKey];

        if(queryType == '+') {
            result.intersect(tmpResult);
        } else if(queryType == '-') {
            result.difference(tmpResult);
        } else {
            result.unionWith(tmpResult);
        }
    }

    return result;
}

/*
 * 根据关键词搜索相应网页内容;
 */
void searchEngine(string dbfile)
{
    cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> index;
    int nPages = buildIndex(dbfile, index);
    cout << "Indexed "<< nPages
         << " pages containing "
         << index.keys().size()
         << " unique terms" << endl;
    cout << endl;

    while(true) {
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if(query.size() == 0)
            break;
        Set<string> pages = findQueryMatches(index, query);
        cout << "Found " << pages.size() << " matching pages " << endl;
        cout << "{";

        int count = 0;
        int resultSize = pages.size();
        for(const string& page: pages) {
            count++;
            if (count < resultSize)
                cout << "\"" << page << "\", ";
            else
                cout << "\"" << page << "\"}" << endl;
        }
        cout << endl;
    }
}

/* * * * * * Test Cases * * * * * */

//PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
//    EXPECT_EQUAL(cleanToken("hello"), "hello");
//    EXPECT_EQUAL(cleanToken("WORLD"), "world");
//    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
//}

//PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
//    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
//    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
//}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here

STUDENT_TEST("test cleanToken") {
    EXPECT_EQUAL(cleanToken("!!he&llo~"), "he&llo");
    EXPECT_EQUAL(cleanToken("ABC*&sD$"), "abc*&sd");
    EXPECT_EQUAL(cleanToken("*&^$@)(@$"), "");
    EXPECT_EQUAL(cleanToken("<<section>>"), "section");
    EXPECT_EQUAL(cleanToken("<section>"), "section");
    EXPECT_EQUAL(cleanToken("<!<&section>@>&"), "section");
}

STUDENT_TEST("gatherTokens from seuss, 2 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("!!he&llo~ ABC*&sD$ *&^$@)(@$ ABC*&sD$ *&^$@)(@$ !!he&llo~");
    EXPECT_EQUAL(tokens.size(), 2);
    EXPECT(tokens.contains("he&llo"));
    EXPECT(tokens.contains("abc*&sd"));
    EXPECT(!tokens.contains("!he&llo"));
}

STUDENT_TEST("time cost of create buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    TIME_OPERATION(nPages, buildIndex("res/tiny.txt", index));
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matches = findQueryMatches(index, "fish milk");
    EXPECT_EQUAL(matches.size(), 3);
    matches = findQueryMatches(index, "fish -milk");
    EXPECT_EQUAL(matches.size(), 2);
    matches = findQueryMatches(index, "fish +milk");
    EXPECT_EQUAL(matches.size(), 1);
}
