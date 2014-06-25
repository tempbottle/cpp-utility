// Copyright 2014 ronaflx
#ifndef STRINGS_SPLIT_H_
#define STRINGS_SPLIT_H_

#include <vector>
#include <string>

#include "strings/string_piece.h"
using std::vector;
using std::string;

vector<string> Split(const string& val, char delim);
vector<string> Split(const string& val, const string& delims);

template<typename Pred>
vector<string> Split(const string& val, char delim, Pred p);
template<typename Pred>
vector<string> Split(const string& val, const string& delims, Pred p);

class SkipEmpty {
 public:
  bool operator ()(const StringPiece& piece) {
    return !piece.empty();
  }
};
#endif  // STRINGS_SPLIT_H_
