// Copyright 2014 ronaflx
// csv parser based on RFC4180
// The ABNF grammar of csv
// file = [header CRLF] record *(CRLF record) [CRLF]
// header = name *(COMMA name)
// record = field *(COMMA field)
// name = field
// field = (escaped / non-escaped)
// escaped = DQUOTE *(TEXTDATA / COMMA / CR / LF / 2DQUOTE) DQUOTE
// non-escaped = *TEXTDATA
// COMMA = %x2C
// CR = %x0D
// DQUOTE =  %x22 ;
// LF = %x0A ;
// CRLF = CR LF ;
// TEXTDATA =  %x20-21 / %x23-2B / %x2D-7E

#ifndef UTIL_CSV_PARSER_H_
#define UTIL_CSV_PARSER_H_
#include <vector>
#include <string>
#include "base/integral_types.h"
#include "base/macros.h"
#include "strings/string_piece.h"
namespace csv {

class Record {
 public:
  const std::vector<std::string>& fields() const {
    return fields_;
  }
  int64 number() const {
    return number_;
  }
 private:
  friend class Parser;

  explicit Record(int64 number)
      : number_(number) {
  }

  int64 number_;  // the ith field of whole csv file
  std::vector<std::string> fields_;
};
class Parser {
 public:
  class Iterator : public std::iterator<std::input_iterator_tag, Record> {
   public:
    Iterator(Parser* parser, bool is_end)
        : parser_(parser),
          is_end_(is_end) {
    }

    Iterator& operator++();

    const Record& operator*() {
      return parser_->record_;
    }

    const Record* operator->() {
      return &parser_->record_;
    }

    // this can be only used with params end()
    bool operator==(const Iterator& that) const {
      bool at_end = is_end_ || !parser_->has_more_data_;
      bool that_at_end = that.is_end_ || !that.parser_->has_more_data_;
      return (parser_ == that.parser_) && (at_end == that_at_end);
    }

    bool operator!=(const Iterator& that) const {
      return !(*this == that);
    }

   private:
    Parser* const parser_;
    bool is_end_;
  };

  typedef Iterator iterator_type;
  typedef const Record value_type;
  iterator_type begin() const {
    return begin_iter_;
  }

  iterator_type end() const {
    return end_iter_;
  }

  explicit Parser(const StringPiece& source);
  Parser(const StringPiece& source, char delim);

 private:
  void Advance();

  StringPiece source_;
  Record record_;
  bool has_more_data_;
  const char delim_;
  int record_count_;
  Iterator begin_iter_;
  Iterator end_iter_;

  DISALLOW_COPY_AND_ASSIGN(Parser);
};

} /* namespace csv */

#endif  // UTIL_CSV_PARSER_H_
