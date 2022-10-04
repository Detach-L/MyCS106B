#include "palindrome.h"

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
