#ifndef RECCHECK
// Include necessary libraries
#include <iostream>
#include <algorithm> 
#include <map>
#include <set>
#endif
#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Declare helper function prototype
void complete(std::set<std::string>& words,
  const std::string& in,
  const std::string& floating,
  const std::set<std::string>& dict);

// Function to initiate the word completion process
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> answer;
    complete(answer, in, floating, dict);
    return answer;
}

// Recursive function to generate words by replacing placeholders
void complete(std::set<std::string>& words,
  const std::string& in,
  const std::string& floating,
  const std::set<std::string>& dict) {
    if (in.find('-') == string::npos) {
        // If no placeholders, check dictionary and add if present
        if (dict.find(in) != dict.end()) {
            words.insert(in);
        }
    }
    else {
        size_t temp = in.find('-');
        // Replace placeholders with floating characters
        for (size_t i = 0; i < floating.size(); i++) {
            string newWord(in);
            newWord[temp] = floating[i];
            string floatString(floating);
            floatString.erase(i, 1);
            complete(words, newWord, floatString, dict);
        }

        // Check if additional letters are needed
        int new_letter = 0;
        for (size_t i = 0; i < in.size(); i++) {
            if (in[i] != '-') {
                new_letter++;
            }
        }
        // Use all letters if more are needed
        if (floating.size() + new_letter < in.size()) {
            for (size_t i = 0; i < 26; i++) {
                string x(1, 'a' + i);
                string newWord(in);
                newWord[temp] = x[0];
                complete(words, newWord, floating, dict);
            }
        }
    }
}
