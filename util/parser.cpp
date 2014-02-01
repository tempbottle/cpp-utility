#include <util/parser.h>

namespace csv {
namespace {
// add a param delim to support tsv files.
inline bool IsTextArea(char next_char, char delim) {
  return ((0x20 <= next_char && next_char <= 0x21)
      || (0x23 <= next_char && next_char <= 0x7E)) && next_char != delim;
}
}
enum ParseState {
  FIELD_START,      // start of a field
  IN_FIELD,         // in the field
  IN_QUOTED_FIELD,  // field with ""
  QUOTED_QUOTE,     // escaped QUOTE
  ERROR,            // error state
  END_OF_RECORD     // end of a field
};

Parser::Iterator& Parser::Iterator::operator++() {
  parser_->Advance();

  if (!parser_->has_more_data_)
    is_end_ = true;
  return *this;
}

Parser::Parser(const StringPiece& source)
    : source_(source),
      record_(0),
      has_more_data_(true),
      delim_(','),
      record_count_(0),
      begin_iter_(this, source.empty()),
      end_iter_(this, true) {
}

Parser::Parser(const StringPiece& source, char delim)
    : source_(source),
      record_(0),
      has_more_data_(true),
      delim_(delim),
      record_count_(0),
      begin_iter_(this, source.empty()),
      end_iter_(this, true) {
}

void Parser::Advance() {
  ParseState state = FIELD_START;
  std::string field;
  record_ = Record(record_count_++);
  while (state != ERROR && state != END_OF_RECORD) {
    if (source_.empty()) {
      // the record is not empty, but the last field is empty
      if (!field.empty() || !record_.fields_.empty()) {
        record_.fields_.push_back(field);
      }
      has_more_data_ = false;
    }

    char next_char = source_[0];
    source_.remove_prefix(1);
    // treat "\r\n" as "\n"
    if (next_char == '\r') {
      if (source_.empty()) {
        next_char = '\n';
      } else if (source_[0] == '\n') {
        source_.remove_prefix(1);
        next_char = '\n';
      }
    }
    switch (state) {
      case FIELD_START:
        if (next_char == '"') {
          state = IN_QUOTED_FIELD;  // escaped
        } else if (next_char == delim_) {
          record_.fields_.push_back(field);
          field.clear();
          state = FIELD_START;
        } else if (next_char == '\n') {
          record_.fields_.push_back(field);
          state = END_OF_RECORD;
        } else {
          field.push_back(next_char);
          state = IN_FIELD;         // non-escaped
        }
        break;

      case IN_FIELD:  // Normal non-escaped field only support TEXTAREA.
        if (next_char == '\n') {
          record_.fields_.push_back(field);
          field.clear();
          state = END_OF_RECORD;
        } else if (next_char == delim_) {
          record_.fields_.push_back(field);
          field.clear();
          state = FIELD_START;
        } else if (IsTextArea(next_char, delim_)) {
          field.push_back(next_char);
        } else {
          state = ERROR;
        }
        break;

      case IN_QUOTED_FIELD:  // Escaped field
        // deal with escaped-char "" or end of QUOTED_FIELD
        if (next_char == '"') {
          state = QUOTED_QUOTE;
        } else if (IsTextArea(next_char, delim_) || next_char == delim_
            || next_char == '\r' || next_char == '\n') {
          field.push_back(next_char);
        } else {
          state = ERROR;
        }
        break;

      case QUOTED_QUOTE:
        if (next_char == '"') {
          field.push_back('"');
          state = IN_QUOTED_FIELD;
        } else if (next_char == delim_) {
          record_.fields_.push_back(field);
          field.clear();
          state = FIELD_START;
        } else if (next_char == '\n') {
          record_.fields_.push_back(field);
          state = END_OF_RECORD;
        } else {
          state = ERROR;
        }
        break;
      case ERROR:
      case END_OF_RECORD:
        break;
    }
  }
}

} /* namespace csv */
