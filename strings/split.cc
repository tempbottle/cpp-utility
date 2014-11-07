// Copyright 2014 ronaflx
#include "strings/split.h"

#include <vector>
#include <string>

#include "strings/string_piece.h"

using std::vector;
using std::string;

vector<StringPiece> Split(const string& val, char delim) {
  vector<StringPiece> res;
  string::size_type pos = 0;
  string::size_type nxt;
  while ((nxt = val.find(delim, pos)) != string::npos) {
    res.push_back(StringPiece(val.data() + pos, nxt - pos));
    pos = nxt + 1;
  }
  res.push_back(StringPiece(val.data() + pos, val.length() - pos));
  return res;
}

vector<StringPiece> Split(const string& val, const string& delims) {
  vector<StringPiece> res;
  string::size_type pos = 0;
  string::size_type nxt;
  while ((nxt = val.find_first_of(delims, pos)) != string::npos) {
    res.push_back(StringPiece(val.data() + pos, nxt - pos));
    pos = nxt + 1;
  }
  res.push_back(StringPiece(val.data() + pos, val.length() - pos));
  return res;
}

template <typename Pred>
vector<StringPiece> Split(const string& val, char delim, Pred p) {
  vector<StringPiece> res;
  string::size_type pos = 0;
  string::size_type nxt;
  while ((nxt = val.find(delim, pos)) != string::npos) {
    StringPiece string_piece(val.c_str() + pos, nxt - pos);
    if (p(string_piece)) {
      res.push_back(string_piece);
    }
    pos = nxt + 1;
  }
  res.push_back(StringPiece(val.data() + pos, val.length() - pos));
  return res;
}
