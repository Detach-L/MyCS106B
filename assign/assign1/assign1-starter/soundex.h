/**
 * File: soundex.h
 *
 * This is just here to communicate the prototype 
 * information for the functions in soundex.cpp that 
 * will be called from main.cpp
 */
#pragma once
#include <string>
#include <vector>

void soundexSearch(std::string filepath);
std::string soundex(std::string s);
std::string removeNonLetters(std::string s);

std::vector<std::string> getSoundexEncoderTable();
std::string encoderStrToSoundexNumber(const std::string& s);
std::string mergeNumEncoder(const std::string& s);
std::string getSoundexFromMergeEncoder(const std::string& s, const std::string& removeStringFirstChar);
