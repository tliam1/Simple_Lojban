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
  static const unordered_map<string, PredType> predicateMap = {
      {"fatci", FACTI},
      {"sumji", SUMJI},
      {"vujni", VUJNI},
      {"dunli", DUNLI},
      {"steni", STENI},
      {"steko", STEKO},
      {"cmavo", CMAVO}
  };

  auto predType = predicateMap.find(word.value); // find() returns the key val
  if (predType != predicateMap.end()) // find returns end() val if found nothing
      return predType->second; // ->second is to access value associated with the key found during find operation
  else
      return NONE;
}

void AssignArgs(vector<Word> words){
  int iterator = 0;
  // will need a vector of arg pointers if we want to do a bunch of operands in a single parse
  arg* args = nullptr; // Pointer to base class
  None* none = new None();
  for (auto& word : words) {
     if(word.value == "i") // ignore i's as it indicates start of command (should be at iterator 0)
       continue;
     if(IsPred(word)){
       PredType predType = GetPredType(word);
       args = InitializePredClass(predType);
     }else if (word.value == "se"){
       //swap the next two parameters to the pred
     }else{
       if (args == nullptr) {
         none->params.push_back(word);
       } else {
         /*
          * if we find a pred and he have some previously stored params
          * apply it to the new pred class
          * otherwise, assign as normal
          * this is to avoid backtracking in our parsing
          */
         if(!none->params.empty()) {
           args->params = move(none->params);
           cout<<"COPIED PARAMS FROM NONE CLASS INTO ARGS. is none now empty? " << none->params.size() <<endl;
         }
         args->params.push_back(word);
       }
     }
     iterator++;
  }
  //  checking
  //  for (auto& items : args->params){
  //    cout<< items.value <<endl;
  //  }
  // call the function in the pred class to run the argument
  delete args;
  delete none;
}


arg* InitializePredClass(PredType pred){
  switch (pred) {
     case FACTI:
       return new Facti();
     case SUMJI:
       return new Sumji();
     case VUJNI:
       return new Vujni();
     case DUNLI:
       return new Dunli();
     case STENI:
       return new Steni();
     case STEKO:
       return new Steko();
     case CMAVO:
       return new Cmavo();
     default:
       cerr << "Error: Unknown predicate type\n";
       exit(1);
   }
}


arg::arg(){
  predType = NONE;
  argCount = 0;
  pval.s = "";
  pval.i = -1;
  pval.cond = false;
  pval.boolFlad = -1;
  pval.list.clear();
  params.clear();
  se_swapper.clear();
}

arg::~arg() {
    // deallocate stuff here ig
}
