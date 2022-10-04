/**
 * Determine if a string is a palindrome!
 */

#include <iostream>
#include "console.h"
#include "simpio.h"  // for getLine
#include "testing/SimpleTest.h"
#include "palindrome.h"

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
                return 0;
    }

    std::string str = getLine("Please enter a string: ");
    if(isPalindrome(str))
        std::cout << "\"" << str << "\" is a palindrome!" << std::endl;
    else
        std::cout << "\"" << str << "\" is not a palindrome!" << std::endl;

    return 0;
}

