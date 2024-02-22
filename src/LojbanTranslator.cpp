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
       w.type = INITIALIZATION;
       w.value = word;
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
bool IsPred(Word word){
  if (word.value == "fatci" || word.value == "sumji" || word.value == "vujni" || word.value == "dunli"
      || word.value == "steni" || word.value == "steko" || word.value == "cmavo") {
      return true;
  }
  return false;
}

PredType GetPredType(Word word){
  return NONE;
}

void AssignArgs(vector<Word> words){
  int iterator = 0;
  arg args = arg();
  for (auto& word : words) {
     if(word.value == "i")
       continue;
     if(IsPred(word)){
       args.predType = GetPredType(word);
     }else if (word.value == "se"){
       //swap the next two parameters to the pred
     }else{
       args.params.push_back(word);
     }
     iterator++;
  }
}

void ProcessArgs(arg args){
  // make if,switch, or other functionality to analyze the enum in the passed in struct
  // nah actually make function pointers in the class that points
  // to the specific class function depending on the pred type
}

arg::arg(){
  predType = NONE;
  argCount = 0;
}




