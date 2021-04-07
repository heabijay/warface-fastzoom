#include <algorithm> 
#include <cctype>
#include <locale>
#include "stringTrim.h"


void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}


std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}


std::string trim_copy(std::string s) {
    trim(s);
    return s;
}