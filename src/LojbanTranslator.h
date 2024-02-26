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
enum ValueType { NUM, STRING, LIST, EMPTY };
// split words when parsing text
struct Word {
  WordType type;
  string value;
};


struct pVal{
  ValueType vType = EMPTY;
  string s = "";
  int i = -1;
  bool cond = false;
  int boolFlad = -1;
  vector<string> list;

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
      cout << "FACTI ERROR: Too many arguments for predicate function" << endl;
      exit(0);
    }

    if(all_of(params[0].value.begin(), params[0].value.end(), ::isdigit)){
      return;
    }
    if (db->database.find(params[0].value) == db->database.end() && params[0].value != "steni"){
      cout<<"FACTI: " << params[0].value << " does not exist";
      exit(0);
    }
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
  }
};

class Steko : public arg {
public:
  Steko() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Steko predicate
  }
};

class Cmavo : public arg {
public:
  Cmavo() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Cmavo predicate
  }
};

class None : public arg {
public:
  None() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation(Storage * db) override {
      // Implementation for Cmavo predicate
  }
};
