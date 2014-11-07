// Copyright 2014 ronaflx
#ifndef STRINGS_SPLIT_H_
#define STRINGS_SPLIT_H_

#include <vector>
#include <string>

#include "strings/string_piece.h"

extern std::vector<StringPiece> Split(const std::string& val, char delim);

extern std::vector<StringPiece> Split(const std::string& val,
                                      const std::string& delims);

template <typename Pred>
std::vector<StringPiece> Split(const std::string& val,
                               const std::string& delims, Pred p) {
  std::vector<StringPiece> res;
  std::string::size_type pos = 0;
  std::string::size_type nxt;
  while ((nxt = val.find_first_of(delims, pos)) != std::string::npos) {
    StringPiece string_piece(val.c_str() + pos, nxt - pos);
    if (p(string_piece)) {
      res.push_back(string_piece);
    }
    pos = nxt + 1;
  }
  res.push_back(StringPiece(val.data() + pos, val.length() - pos));
  return res;
}

class SkipEmpty {
 public:
  bool operator()(const StringPiece& piece) { return !piece.empty(); }
};

#endif  // STRINGS_SPLIT_H_
