#ifndef UTIL_CSV_WRITER_H_
#define UTIL_CSV_WRITER_H_
#include <vector>
#include <string>
#include <iostream>

#include "base/macros.h"
namespace csv {
// add a param delim to support tsv files.
inline bool IsTextArea(char next_char, char delim) {
  return ((0x20 <= next_char && next_char <= 0x21)
      || (0x23 <= next_char && next_char <= 0x7E)) && next_char != delim;
}
template<typename P>
bool GetField(const P& element, char delim, std::ostream* out) {
  (*out) << element;
  return true;
}

template<>
bool GetField<std::string>(const std::string& element, char delim,
                           std::ostream* out) {
  bool is_all_textarea = true;
  for (const auto& i : element) {
    if (!IsTextArea(i, delim)) {
      is_all_textarea = false;
      break;
    }
  }
  if (is_all_textarea) {
    (*out) << element;
  } else {
    (*out) << "\"";
    for (const auto& i : element) {
      if (i == '\"') {
        (*out) << "\"\"";
      } else if (IsTextArea(i, delim) || i == '\r' || i == '\n' || i == delim) {
        (*out) << i;
      } else {
        fprintf(stderr, "%s %d: invalid character\n", __FILE__, __LINE__);
        return false;
      }
    }
    (*out) << "\"";
  }
  return true;
}

template<typename T, typename Container = std::vector<T> >
class Writer {
 public:
  Writer(const Container& container, std::ostream* out)
      : container_(&container),
        delim_(','),
        out_(out) {
  }

  Writer(const Container& container, char delim, std::ostream* out)
      : container_(&container),
        delim_(delim),
        out_(out) {
  }

  bool WriteRecord() const {
    bool has_first = false;
    for (const auto& element : *container_) {
      if (has_first) {
        (*out_) << delim_;
      }
      if (!GetField(element, delim_, out_)) {
        return false;
      }
      has_first = true;
    }
    (*out_) << '\n';
    return true;
  }

 private:
  const Container* container_;
  char delim_;
  std::ostream* out_;

  DISALLOW_COPY_AND_ASSIGN(Writer);
};
} /* namespace csv */

#endif  // UTIL_CSV_WRITER_H_
