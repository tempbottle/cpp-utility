#include "strings/split.h"

#include <vector>
#include <string>

#include "strings/string_piece.h"

vector<string> Split(const string& val, char delim) {
  vector<string> res;
  string::size_type pos = 0;
  string::size_type nxt;
  while ((nxt = val.find(delim, pos)) != string::npos) {
    res.push_back(val.substr(pos, nxt - pos));
    pos = nxt + 1;
  }
  return res;
}

vector<string> Split(const string& val, const string& delims) {
  vector<string> res;
  string::size_type pos = 0;
  string::size_type nxt;
  while ((nxt = val.find_first_of(delims, pos)) != string::npos) {
    res.push_back(val.substr(pos, nxt - pos));
    pos = nxt + 1;
  }
  return res;
}

template<typename Pred>
vector<string> Split(const string& val, char delim, Pred p) {
  vector<string> res;
  string::size_type pos = 0;
  string::size_type nxt;
  while ((nxt = val.find(delim, pos)) != string::npos) {
    StringPiece string_piece(val.c_str() + pos, nxt - pos);
    if (p(string_piece)) {
      res.push_back(string_piece.ToString());
    }
    pos = nxt + 1;
  }
  return res;
}
template<typename Pred>
vector<string> Split(const string& val, const string& delims, Pred p) {
  vector<string> res;
  string::size_type pos = 0;
  string::size_type nxt;
  while ((nxt = val.find_first_of(delims, pos)) != string::npos) {
    StringPiece string_piece(val.c_str() + pos, nxt - pos);
    if (p(string_piece)) {
      res.push_back(string_piece.ToString());
    }
    pos = nxt + 1;
  }
  return res;
}
