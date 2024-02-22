//============================================================================
// Name        : Simple.cpp
// Author      : Liam Tapper
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "LojbanTranslator.h"
using namespace std;

int main() {
  unordered_map<string, pVal> database; // Database to store predicates and their arguments
  string statement;
  while (getline(cin, statement)) {
    vector<Word> words = split(statement);

    // Process the statement (WHERE A LOT OF THE COMPUTATION WILL EXIST)
    // place helper functions in .h file
    if (words[0].value == "i") {
      // Check if the statement is declaring a predicate
      AssignArgs(words);
    }
  }

  return 0;
}
