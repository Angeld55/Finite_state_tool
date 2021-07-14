#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../AcyclicSubsequentialTransducer/SubsequentialTransducer.h"

SubsequentialTransducer createMinimalSubsequentialTransducerBySortedFile(const vector<pair<string, string>>& input, size_t maxWordLength);

void insertWord(const string& word, size_t outPut, SubsequentialTransducer& st);

void removeWord(const string& word, SubsequentialTransducer& st);