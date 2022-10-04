/**
 * Determine if a string is a palindrome!
 */

#include <iostream>
#include "console.h"
#include "simpio.h"  // for getLine
#include "palindrome.h"

int main() {
    std::string str = getLine("Please enter a string: ");
    if(isPalindrome(str))
        std::cout << "\"" << str << "\" is a palindrome!" << std::endl;
    else
        std::cout << "\"" << str << "\" is not a palindrome!" << std::endl;

    return 0;
}

