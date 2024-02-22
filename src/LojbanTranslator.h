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
//"fatci" "sumji""vujni" "dunli" "steni" "steko" "cmavo")
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

    vector<Word> params;
    vector<vector<int>> se_swapper;
    struct pVal pval;
    PredType predType;
    int argCount;
};


vector<Word> split(const string& statement);
void AssignArgs(vector<Word> words);
bool IsPred(Word word);
PredType GetPredType(Word word);
void ProcessArgs(arg args);
//FACTI, SUMJI, VUJNI, DUNLI, STENI, STEKO, CMAVO, NONE
void Facti(arg args);
void Sumji(arg args);
void Vujni(arg args);
void Dunli(arg args);
void Steni(arg args);
void Steko(arg args);
void Cmavo(arg args);
