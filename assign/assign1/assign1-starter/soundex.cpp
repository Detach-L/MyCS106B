/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!  Debug!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* This function is intended to return a vector which
 * contains a Soundex encoder table.
 */
vector<string> getSoundexEncoderTable() {
    vector<string> v;
    v.push_back("A E I O U H W Y");
    v.push_back("B F P V");
    v.push_back("C G J K Q S X Z");
    v.push_back("D T");
    v.push_back("L");
    v.push_back("M N");
    v.push_back("R");
    return v;
}

/* This function is intended to return a Soundex encoder of a given string.
 */
string encoderStrToSoundexNumber(const string& s) {
    string result = "";
    if(s.length() == 0)
        return result;
    vector<string> v = getSoundexEncoderTable();
    for (int i = 0; i < s.length(); i++) {
        for(int j=0; j < v.size(); j++){
            if( v.at(j).find(toUpperCase(s.at(i))) != string::npos)
                result += integerToString(j);
        }
    }
    return result;
}

/*
 * This function is intended to return a string which
 * merges the adjacent repeated numbers.
 */
string mergeNumEncoder(const string& s) {
    string result = "";
    if(s.length() == 0)
        return result;
    result += s.at(0);
    for (int i = 1; i < s.length(); i++) {
        if(s.at(i) == result.at(result.length()-1))
            continue;
        else
            result += s.at(i);
    }
//    result.at(0) = toUpperCase(s.at(0));
    return result;
}

/*
 * This function is intended to return a string which
 * replace the first number by the first character of a given string and
 * deletes all '0' in a given string and
 * makes the length of the code exactly 4 by padding 0 or truncating the excess.
 */
string getSoundexFromMergeEncoder(const string& s, const string& removeStringFirstChar) {
    string result = "";
    if(s.length() == 0)
        return result;
    result.append(charToString(toUpperCase(removeStringFirstChar.at(0))));
    for (int i = 1; i < s.length(); i++) {
        if(s.at(i) == '0')
            continue;
        else
            result += s.at(i);
    }
    while(result.length() < 4){
        result.append("0");
    }
    return result.substr(0, 4);
}


/* This function is intended to return a Soundex encoder.
 */
string soundex(string s) {
    string result = "";
    result = removeNonLetters(s);
    string firstCharcter = charToString(result.at(0));  // string must after remove
    result = encoderStrToSoundexNumber(result);
    result = mergeNumEncoder(result);
    result = getSoundexFromMergeEncoder(result, firstCharcter);
    return result;
}


/* This function is intended to encode a given string to Soundex encoder and
 * find the names that have the same encoder in database.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    while(true){
        string soundexCode = getLine("Enter a surname (RETURN to quit): ");
        if(soundexCode.length() == 0)
            break;

        soundexCode = soundex(soundexCode);
        Vector<string> sameNames;
        for (Vector<string>::iterator it = databaseNames.begin(); it != databaseNames.end(); it++){
            if(soundexCode == soundex(*it))
                sameNames.add(*it);
        }
        sameNames.sort();

        string toPrintNames = "";
        for (Vector<string>::iterator it = sameNames.begin(); it != sameNames.end(); it++){
                toPrintNames += "\"" + (*it) + "\", ";
        }
        cout << "Soundex code is " << soundexCode << endl;
        if(toPrintNames.length() != 0){
            toPrintNames.at(toPrintNames.length() - 1) = '}';  // replace the last ',' to '}'
            cout << "Matches from database: {" << toPrintNames << endl;
        }else{
            cout << "No matches from database!" << endl;
        }
    }
    cout << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

STUDENT_TEST("Test removing puntuation, digits, and spaces version 2") {
    string s = "'OHara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "'0'13'";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Test encoder string to Soundex") {
    string s = "Curie";
    string result = encoderStrToSoundexNumber(s);
    EXPECT_EQUAL(result, "20600");
    s = "OConner";
    result = encoderStrToSoundexNumber(s);
    EXPECT_EQUAL(result, "0205506");
    s = "";
    result = encoderStrToSoundexNumber(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Test merge the adjacent repeated numbers of encoder string") {
    string s = "222025";
    string result = mergeNumEncoder(s);
    EXPECT_EQUAL(result, "2025");
    s = "0000000";
    result = mergeNumEncoder(s);
    EXPECT_EQUAL(result, "0");
    s = "200033331512101111";
    result = mergeNumEncoder(s);
    EXPECT_EQUAL(result, "2031512101");
}

STUDENT_TEST("Test merge the adjacent repeated numbers of encoder string") {
    string s = "02025";
    string result = getSoundexFromMergeEncoder(s, "Cqwf");
    EXPECT_EQUAL(result, "C225");
    s = "C0";
    result = getSoundexFromMergeEncoder(s, "Cqwf");
    EXPECT_EQUAL(result, "C000");
    s = "C203010";
    result = getSoundexFromMergeEncoder(s, "Cqwf");
    EXPECT_EQUAL(result, "C231");
}

STUDENT_TEST("Test merge the adjacent repeated numbers of encoder string") {
    string s = "Zelenski";
    string result = soundex(s);
    EXPECT_EQUAL(result, "Z452");
    s = "hanrahan";
    result = soundex(s);
    EXPECT_EQUAL(result, "H565");
    s = "Curie";
    result = soundex(s);
    EXPECT_EQUAL(result, "C600");
    s = "O'Conner";
    result = soundex(s);
    EXPECT_EQUAL(result, "O256");
}

