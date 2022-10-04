#ifndef PALINDROME_H
#define PALINDROME_H

#include<string>

bool isPalindrome(const std::string& str);
bool isPalindromeRecursive(const std::string& str);
//bool isPalindromeRecursiveBinarySearch(const std::string& str);
bool isPalindromeRecursive(const std::string& str, int start, int end);

#endif // PALINDROME_H
