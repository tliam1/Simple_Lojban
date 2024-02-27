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
  // put this map into a class and have a pointer to the class so we can pass it around nicely
  cout << "Notice: This program currently takes one predicate at a time (predicate names are fine)\n"
      "Furthermore, this program will dump all information about names or user-defined predicates after simulating EOF" << endl;

  Storage* db = new Storage();
  string statement;
  while (getline(cin, statement)) {
    vector<Word> words = split(statement);

    // Process the statement (WHERE A LOT OF THE COMPUTATION WILL EXIST)
    // place helper functions in .h file
    if (words[0].value == "i") {
      // Check if the statement is declaring a predicate
      AssignArgs(words, db);
    }
  }
  db->printAlteredValues();
  return 0;
}
