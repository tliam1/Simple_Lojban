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
enum WordType { SHORT_WORD, PREDICATE_WORD, NUMBER, NAME };

// split words when parsing text
struct Word {
    WordType type;
    string value;
};

vector<Word> split(const string& statement);

