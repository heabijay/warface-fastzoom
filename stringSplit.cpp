#include "stringSplit.h"

std::vector<std::string> stringSplit(std::string string, char delimiter)
{
    std::vector<std::string> array;
    int l = string.size();
    int wordStart = 0;
    int selectedLength;
    for (int i = 0; ; i++)
    {
        if (i > l || string[i] == delimiter) {
            selectedLength = i - wordStart;
            if (selectedLength > 0) {
                array.push_back(string.substr(wordStart, selectedLength));
            }

            wordStart = i + 1;

            if (i > l)
                break;
        }
    }

    return array;
}