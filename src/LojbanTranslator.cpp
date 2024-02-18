/*
 * LojbanTranslator.cpp
 *
 *  Created on: Feb 17, 2024
 *      Author: liamk
 */
#include "LojbanTranslator.h"


vector<Word> split(const string& statement){
  vector<Word> words;
  /*
   * istringstream is a class provided by the C++ Standard Library
   * creates a class with name iss and passes the string to the constructor
   * treat a string as if it were an input stream
   * Streams till white space is encountered (then looped for every word)
   */
  istringstream iss(statement);
  string word;
  while (iss >> word) {
     Word w;
     cout << word << endl;
     if (word == "lo" || word == "se") {
       w.type = SHORT_WORD;
       w.value = word;
     } else if (word == "fatci" || word == "sumji" || word == "vujni" || word == "dunli" || word == "steni" || word == "steko" || word == "cmavo") {
       w.type = PREDICATE_WORD;
       w.value = word;
     } else if (all_of(word.begin(), word.end(), ::isdigit)) {
       w.type = NUMBER;
       w.value = word;
     } else if (word.front() == '.' && word.back() == '.') {
       w.type = NAME;
       w.value = word.substr(1, word.size() - 2);
     } else if (word == "i"){
       cout << "Found 'i'. Starting Command Processing" << endl;
     } else {
       // Error: Invalid word
       cerr << "Error: Invalid word \"" << word << "\" found in the statement\n";
       exit(1);
     }
     words.push_back(w);
  }
  return words;
}


