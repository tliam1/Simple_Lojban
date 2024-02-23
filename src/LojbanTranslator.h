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

// split words when parsing text
struct Word {
  WordType type;
  string value;
};


struct pVal{
  string s = "";
  int i = -1;
  bool cond = false;
  int boolFlad = -1;
  vector<string> list;
};

class arg{
  public:
    arg();
    virtual ~arg(); // virtual destructor
    vector<Word> params;
    vector<vector<int>> se_swapper;
    struct pVal pval;
    PredType predType;
    int argCount;

    virtual void predOperation() = 0; // Declaring abstract virtual function
};


vector<Word> split(const string& statement);
void AssignArgs(vector<Word> words);
bool IsPred(Word word);
PredType GetPredType(Word word);
arg* InitializePredClass(PredType pred);
//FACTI, SUMJI, VUJNI, DUNLI, STENI, STEKO, CMAVO, NONE
class Facti : public arg {
public:
  Facti() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Facti predicate
  }
};

class Sumji : public arg {
public:
  Sumji() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Sumji predicate
  }
};

class Vujni : public arg {
public:
  Vujni() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Vujni predicate
  }
};


class Dunli : public arg {
public:
  Dunli() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Dunli predicate
  }
};

class Steni : public arg {
public:
  Steni() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Steni predicate
  }
};

class Steko : public arg {
public:
  Steko() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Steko predicate
  }
};

class Cmavo : public arg {
public:
  Cmavo() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Cmavo predicate
  }
};

class None : public arg {
public:
  None() : arg() {
      // Additional initialization specific to Vujni class, if needed
  }
  void predOperation() override {
      // Implementation for Cmavo predicate
  }
};
