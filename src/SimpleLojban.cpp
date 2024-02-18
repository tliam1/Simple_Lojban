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
  unordered_map<string, vector<string>> database; // Database to store predicates and their arguments
  string statement;
  while (getline(cin, statement)) {
      vector<Word> words = split(statement);
      // Handle the last statement
      if (cin.peek() == EOF) {
          for (const auto& word : words) {
              if (word.type == NAME) {
                  cout << word.value << endl;
              }
          }
          break;
      }
      // Process the statement (WHERE A LOT OF THE COMPUTATION WILL EXIST)
      // place helper functions in .h file
      if (words[0].value == "i") {
          // Check if the statement is declaring a predicate
          if (words[1].type == PREDICATE_WORD && words[2].type == PREDICATE_WORD && words[3].type == NAME) {
              string predicateName = words[2].value;
              string argument = words[3].value;
              database[predicateName]=argument;
          }
      }
  }

  return 0;
}
