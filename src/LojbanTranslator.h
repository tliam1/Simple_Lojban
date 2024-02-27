/*
 * LojbanTranslator.h
 *
 *  Created on: Feb 17, 2024
 *      Author: liamk
 */
#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

// used for defining what type the word is
enum WordType { SHORT_WORD, PREDICATE_WORD, NUMBER, NAME, INITIALIZATION };
enum PredType { FACTI, SUMJI, VUJNI, DUNLI, STENI, STEKO, CMAVO, NONE };
enum ValueType { NUM, STRING, LIST, EMPTY, USER_PRED };
// split words when parsing text
struct Word {
  WordType type;
  string value;
};


struct pVal{
  ValueType vType = EMPTY;
  string s = "";
  int i = -1;
  vector<string> list;
  vector<vector<string>> uP; // user defined predicate

  bool operator==(const pVal& other) const {
      return vType == other.vType &&
             s == other.s &&
             i == other.i &&
             list == other.list;
  }
};

class Storage{
  public:
    Storage();
    unordered_map<string, pVal> database;

    void printAlteredValues();
};

class arg{
  public:
    arg();
    virtual ~arg(); // virtual destructor
    vector<Word> params;
    vector<vector<int>> se_swapper;
    struct pVal pval;
    PredType predType;

    virtual void predOperation(Storage * db) = 0; // Declaring abstract virtual function
    bool findInStorage(Storage * db, Word word);
};


vector<Word> split(const string& statement);
void AssignArgs(vector<Word> words, Storage * db);
bool IsPred(Word word);
PredType GetPredType(Word word);
arg* InitializePredClass(PredType pred);
//FACTI, SUMJI, VUJNI, DUNLI, STENI, STEKO, CMAVO, NONE
class Facti : public arg {
public:
  Facti() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Facti predicate
    if(params.size() > 1){
      cout << "FACTI ERROR: Too many/little arguments for predicate function" << endl;
      exit(0);
    }

    if(all_of(params[0].value.begin(), params[0].value.end(), ::isdigit)){
      cout << "FACTI : returns True" << endl;
      return;
    }
    if (db->database.find(params[0].value) == db->database.end() && params[0].value != "steni"){
      cout<<"FACTI: " << params[0].value << " has been created and returns true";
      pVal newPVal;
      db->database[params[0].value] = newPVal;
      return;
    }
    cout << "FACTI : returns True" << endl;
  }
};

class Sumji : public arg {
public:
  Sumji() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
    // Implementation for Sumji predicate
    if (params.size() != 3) {
        cerr << "SUMJI ERROR: Invalid number of arguments for Sumji predicate\n";
        return;
    }
    if(se_swapper.size() > 0){
      for (const auto& swap : se_swapper) {
        // Check if the swap indexes are within the bounds of params
        if (swap.size() == 2 && swap[0] < params.size() && swap[1] < params.size()) {
          // Swap the arguments at the specified indexes
          std::swap(params[swap[0]], params[swap[1]]);
        } else {
          cerr << "SUMJI Error: Invalid swap indexes in se_swapper\n";
        }
      }
    }

    int result = 0;
    unordered_map<int, int> op;
    int it = 0;
    string unknownWord = "";
    //set up calculatio
    for (auto& word : params) {
      if (word.type == NAME){
        if(!findInStorage(db, word)){
          // db->database.insert({word.value, pval});
          unknownWord = word.value;
        }else{
          op.insert({it, db->database[word.value].i});
        }
      }else if (word.type == NUMBER){
        op.insert({it, stoi(word.value)});
      }
      it++;
    }

    //process calculation
    if (op.size() < 2){
      cout << "SUMJI ERROR: Not enough knowns for operation" << endl;
    }
    if(op.size() == 3){
      if(op[0] == (op[1] + op[2])){
        cout<<"SUMJI: Operation is true"<<endl;
        return;
      }
      cout<<"SUMJI: Operation is false"<<endl;
      return;
    }
    if (op.find(1) != op.end() && op.find(2) != op.end()){
      result = op[1] + op[2];
      pVal newPVal;
      newPVal.i = result;
      newPVal.vType = NUM;
      db->database[unknownWord] = newPVal;
    }else if (op.find(0) != op.end() && op.find(2) != op.end()){
      result = op[0] - op[2];
      pVal newPVal;
      newPVal.i = result;
      newPVal.vType = NUM;
      db->database[unknownWord] = newPVal;
    }
    else if (op.find(0) != op.end() && op.find(1) != op.end()){
      result = op[0] - op[1];
      pVal newPVal;
      newPVal.i = result;
      newPVal.vType = NUM;
      db->database[unknownWord] = newPVal;
    }
    cout<<"SUMJI: Result " << result << " is now stored in " << unknownWord << endl;
  }
};

class Vujni : public arg {
public:
  Vujni() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
     // Implementation for Vujni predicate
    if (params.size() != 3) {
        cerr << "VUJNI ERROR: Invalid number of arguments for VUJNI predicate\n";
        return;
    }
    if(se_swapper.size() > 0){
      for (const auto& swap : se_swapper) {
        // Check if the swap indexes are within the bounds of params
        if (swap.size() == 2 && swap[0] < params.size() && swap[1] < params.size()) {
          // Swap the arguments at the specified indexes
          std::swap(params[swap[0]], params[swap[1]]);
        } else {
          cerr << "VUJNI Error: Invalid swap indexes in se_swapper\n";
        }
      }
    }

    int result = 0;
    unordered_map<int, int> op;
    int it = 0;
    string unknownWord = "";
    //set up calculations
    for (auto& word : params) {
      pVal pval;
      if (word.type == NAME){
        if(!findInStorage(db, word)){
          // db->database.insert({word.value, pval});
          unknownWord = word.value;
        }else{
          op.insert({it, db->database[word.value].i});
        }
      }else if (word.type == NUMBER){
        op.insert({it, stoi(word.value)});
      }
      it++;
    }

    //process calculation
    if (op.size() < 2){
      cout << "VUJNI ERROR: Not enough knowns for operation" << endl;
    }
    if(op.size() == 3){
      if(op[0] == (op[1] - op[2])){
        cout<<"VUJNI: Operation is true"<<endl;
        return;
      }
      cout<<"VUJNI: Operation is false"<<endl;
      return;
    }
    if (op.find(1) != op.end() && op.find(2) != op.end()){
      result = op[1] - op[2];
      pVal newPVal;
      newPVal.i = result;
      newPVal.vType = NUM;
      db->database[unknownWord] = newPVal;
    }else if (op.find(0) != op.end() && op.find(2) != op.end()){
      result = op[0] + op[2];
      pVal newPVal;
      newPVal.i = result;
      newPVal.vType = NUM;
      db->database[unknownWord] = newPVal;
    }
    else if (op.find(0) != op.end() && op.find(1) != op.end()){
      result = op[0] - op[1];
      pVal newPVal;
      newPVal.i = result;
      newPVal.vType = NUM;
      db->database[unknownWord] = newPVal;
    }
    cout<<"VUJNI: Result " << result << " is now stored in " << unknownWord << endl;
  }
};


class Dunli : public arg {
public:
  Dunli() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Dunli predicate
    if(params.size() != 2){
      cout << "DUNLI ERROR: Invalid Number of arguments" << endl;
      return;
    }
    if(se_swapper.size() > 0){
      for (const auto& swap : se_swapper) {
        // Check if the swap indexes are within the bounds of params
        if (swap.size() == 2 && swap[0] < params.size() && swap[1] < params.size()) {
          // Swap the arguments at the specified indexes
          std::swap(params[swap[0]], params[swap[1]]);
        } else {
          cerr << "DUNLI Error: Invalid swap indexes in se_swapper, ignoring swap operation";
        }
      }
    }
    vector<pVal> namedItems;
    int index = 0;
    for (auto& word : params) {
      if (word.type == NAME){
        if(!findInStorage(db, word)){
          cout<<"DUNLI Error: Cannot compare with unknown word " << word.value << endl;
        }else{
          namedItems.push_back(db->database[word.value]);
          if(index > 0){
            //swap for easy use later
            Word tempWord;
            tempWord = word;
            params[1] = params[0];
            params[0] = tempWord;
          }
        }
      }
      index++;
    }
    if (namedItems.size() == 2){
      if(namedItems[0] == namedItems[1]){
        cout<<"DUNLI: Found equivalence" << endl;
        return;
      }
      cout<<"DUNLI: Found non-equivalence" << endl;
      return;
    }else if (namedItems.size() == 0){
      try {
        if(params[0].value == params[1].value){
          cout<<"DUNLI: Found equivalence" << endl;
          return;
        }else{
          cout<<"DUNLI: Found non-equivalence" << endl;
          return;
        }
      } catch (const std::exception& e) {
          // Catch and handle the exception
        cout<<"DUNLI: Found non-equivalence" << endl;
        return;
      }
    }else if (namedItems.size() == 1){
      pVal opVal = namedItems[0];
      if (opVal.vType == STRING && opVal.s == params[1].value){
        cout<<"DUNLI: Found equivalence" << endl;
        return;
      }else if (opVal.vType == NUM && opVal.i == stoi(params[1].value)){
        cout<<"DUNLI: Found equivalence between " << opVal.i << " and " << stoi(params[1].value) << endl;
        return;
      }else if (opVal.vType == LIST){ //&& opVal.list == params[1].value
        vector<string> holder;
        // I am not sure if this can even happen
        cout<<"DUNLI: Found equivalence" << endl;
        return;
      }
      cout<<"DUNLI: Found non-equivalence" << endl;
      return;
    }
  }
};

class Steni : public arg {
public:
  Steni() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Steni predicate
    if(params.size() != 1){
      cout << "STENI ERROR: Invalid Number of arguments -->" << params.size() << endl;
      return;
    }
    if(params[0].type != NAME){
      cout << "STENI ERROR: Provided parameter is not a name!" << endl;
      return;
    }

    if(se_swapper.size() > 0){
      for (const auto& swap : se_swapper) {
        // Check if the swap indexes are within the bounds of params
        if (swap.size() == 2 && swap[0] < params.size() && swap[1] < params.size()) {
          // Swap the arguments at the specified indexes
          std::swap(params[swap[0]], params[swap[1]]);
        } else {
          cerr << "STENI Error: Invalid swap indexes in se_swapper, ignoring swap operation";
        }
      }
    }

    pVal newPVal;
    newPVal.vType = LIST;
    db->database[params[0].value] = newPVal;
    cout << "STENI : Assigned " << params[0].value << " To an empty list";
  }
};

class Steko : public arg {
public:
  Steko() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Steko predicate

    if(se_swapper.size() > 0){
      for (const auto& swap : se_swapper) {
        // Check if the swap indexes are within the bounds of params
        if (swap.size() == 2 && swap[0] < params.size() && swap[1] < params.size()) {
          // Swap the arguments at the specified indexes
          std::swap(params[swap[0]], params[swap[1]]);
        } else {
          cerr << "STEKO Error: Invalid swap indexes in se_swapper, ignoring swap operation";
        }
      }
    }

    if(params[0].type != NAME){
      //cout << "STENI ERROR: Provided parameter is not a name!" << endl;
      //return;
      // just forms a literal
    }else{
      pVal newPVal;
      newPVal.vType = LIST;
      vector<string> newList;
      /*
       * for each parameter in list other than first param --> add it to pval list
       */
      bool isListItem = false;
      for (auto& word : params) {
        if(word.value == "steni"){
          break;
        }
        if(word.value == "steko"){
          isListItem = true;
          continue;
        }else if (isListItem){
          newList.push_back(word.value);
        }
      }
      if(params[0].type == NAME){
        db->database[params[0].value] = newPVal;
        cout << "STEKO: Added new list. " << params[0].value <<  " contains: ";
        for (size_t i = 1; i < params.size(); ++i) {
          cout << params[i].value << " ";
        }
        cout << endl;
        return;
      }else{
        cout << "STEKO: Created list with no name assignment. List contains: ";
        for (const auto& word : params) {
          cout << word.value << " ";
        }
        cout << endl;
      }
    }
  }
};

/*
 * declaring a predicate. The first argument is the name of the predicate
 * (which can be either a predicate word or a name word, in both cases preceded by the short word lo).
 * The second argument is the argument to the predicate (which can be either a name or a list of names).
 * There must be at least one argument (otherwise, fatci should be used),
 * and you do not have to support lists of arguments longer than five.
 * The last argument is a predicate or list of predicates asserted when the declared predicate is used,
 * which can be the empty list (i.e., no further action).
 * The list of predicates should not have a hard-coded limit.
 * For example, the Prolog statement “parent(Brook, George).”
 * can be declared in this language by
 * “i lo .parent. cmavo lo steko lo .Brook. lo steko lo .George. lo steni”.
 * Here the last argument is empty - nothing further is asserted when this predicate is used.
 * The Prolog statement “grandparent(X,Y) :- parent(X,Z), parent(Z,Y).”
 * can be declared in this language as
 * “i lo .grandparent. cmavo lo steko lo .X. lo steko .Y. lo steni  lo steko lo .X. .parent. lo .Z.
 * lo steko lo .Z. .parent. lo .Y. lo steni”
 */
class Cmavo : public arg {
public:
  Cmavo() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
     // Implementation for Cmavo predicate
    vector<string> argumentToPredicate;
    vector<string> assertedPredicates;
    pVal newPVal;
    newPVal.vType = USER_PRED;

    if(se_swapper.size() > 0){
      for (const auto& swap : se_swapper) {
        // Check if the swap indexes are within the bounds of params
        if (swap.size() == 2 && swap[0] < params.size() && swap[1] < params.size()) {
          // Swap the arguments at the specified indexes
          std::swap(params[swap[0]], params[swap[1]]);
        } else {
          cerr << "Cmavo Error: Invalid swap indexes in se_swapper, ignoring swap operation";
        }
      }
    }

    if(params.size() == 1){
      if(params[0].type != NAME){
        cout << "CMAVO ERROR: First argument should be a name or predicate name" << endl;
        return;
      }else{
        argumentToPredicate.push_back("facti");
        // add to db
      }
      return;
    }
    bool endOfFirstList = false;
    for (auto& word : params) {
      if(word.value == params[0].value){
        //create db item or ignore idk yet
        continue;
      }
      if(!endOfFirstList){ // becomes true when
        if(word.value == "steni"){
          endOfFirstList = true;
          continue;
        }else if (word.value != "steko"){
          argumentToPredicate.push_back(word.value);
        }
      }else{
        if(word.value == "steni"){
          assertedPredicates.push_back(word.value);
          break;
        }
        assertedPredicates.push_back(word.value);
      }
    }
    newPVal.uP.push_back(argumentToPredicate);
    if (!assertedPredicates.empty()) {
      // Create a new row in uP for each asserted predicate + params
//      for (const auto& pred : assertedPredicates){
//        cout << pred << endl;
//      }
      vector<string> newList;
      for (const auto& pred : assertedPredicates) {
        if(pred != "steko" && pred != "steni") {
          newList.push_back(pred);
        } else {
          if (!newList.empty()) {
            newPVal.uP.push_back(newList);
            newList.clear();
          }
        }
      }
      cout << "CMAVO: Added asserted predicates to uP in pVal struct" << endl;
    } else {
      cout << "CMAVO: No asserted predicates found" << endl;
    }

    db->database[params[0].value] = newPVal;
    cout << "CMAVO: Added user-Defined Predicate to Storage" << endl;
  }
};

class None : public arg {
public:
  None() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Cmavo predicate
      // will run if only lo's are ran
    if(params.empty()){
      return;
    }
    for (auto& word : params) {
      pVal newPVal;
      db->database[word.value] = newPVal;
    }
  }
};
