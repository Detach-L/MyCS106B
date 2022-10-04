#include "palindrome.h"
#include "testing/SimpleTest.h"

/*
 * @str: a string to be determined whether is a palindrome;
 * @return: if str is a palindrome, return true, else return false;
 */
bool isPalindrome(const std::string& str){
//    if(str.length() == 0)
//        return false;
    for(int i=0; i < str.length()/2; i++){
        if(std::tolower(str.at(i)) != std::tolower(str.at(str.length() -1 - i)))
            return false;
    }
    return true;
}

bool isPalindromeRecursive(const std::string& str) {
    // edge case
    if(str.size() <= 1){  // empty string "" or only have one character "a"
        return true;
    }

    // pick the first and last character
    char first = std::tolower(str.at(0));
    char last = std::tolower(str.at(str.size() - 1));
    if (first != last)
        return false;

    // check the rest string whether is Palindrome or not (someone do it for you)
    const std::string& subStr = str.substr(1, str.size() - 2);  // 此处-2是切实拿到三个值
    if (isPalindromeRecursive(subStr))
        return true;
    return false;
}

//bool isPalindromeRecursiveBinarySearch(const std::string& str) {
//    // edge case


//    // pick the first and last character


//    // check the rest string whether is Palindrome or not (someone do it for you)


//    return false;
//}

bool isPalindromeRecursive(const std::string& str, int start, int end) {
    // edge case
    if(end - start <= 1){  // empty string "" or only have one character "a"
        return true;
    }

    // pick the first and last character
    char first = std::tolower(str.at(start));
    char last = std::tolower(str.at(end - 1));  // cat acess index end
    if (first != last)
        return false;

    // check the rest string whether is Palindrome or not (someone do it for you)
    if (isPalindromeRecursive(str, start + 1, end - 1))  // 此处-1是取不到end
        return true;
    return false;
}


// TODO: add your student test cases here

STUDENT_TEST("Test isPalindrome") {
    std::string s = "level";
    EXPECT(isPalindrome(s));
    s = "a";
    EXPECT(isPalindrome(s));
    s = "";
    EXPECT(isPalindrome(s));
    s = "aatta";
    EXPECT(!isPalindrome(s));
}

STUDENT_TEST("Test isPalindrome") {
    std::string s = "level";
    EXPECT(isPalindromeRecursive(s));
    s = "a";
    EXPECT(isPalindromeRecursive(s));
    s = "";
    EXPECT(isPalindromeRecursive(s));
    s = "aatta";
    EXPECT(!isPalindromeRecursive(s));
}

STUDENT_TEST("Test isPalindrome") {
    std::string s = "level";
    EXPECT(isPalindromeRecursive(s, 0, s.size()));
    s = "a";
    EXPECT(isPalindromeRecursive(s, 0, s.size()));
    s = "";
    EXPECT(isPalindromeRecursive(s, 0, s.size()));
    s = "aatta";
    EXPECT(!isPalindromeRecursive(s, 0, s.size()));
}


STUDENT_TEST("Time trials of isPalindrome") {
    std::string s = "aaaaaaaaaaa";
    for(int i=0; i < 10; i++){
       s += s;
    }
    TIME_OPERATION(s.size(), isPalindrome(s));
}

STUDENT_TEST("Time trials of isPalindrome") {
    std::string s = "aaaaaaaaaaa";
    for(int i=0; i < 4; i++){
       s.append(s);
    }
    TIME_OPERATION(s.size(), isPalindromeRecursive(s));
}

STUDENT_TEST("Time trials of isPalindrome") {
    std::string s = "aaaaaaaaaaa";
    for(int i=0; i < 10; i++){
       s.append(s);
    }
    TIME_OPERATION(s.size(), isPalindromeRecursive(s, 0, s.size()));
}

