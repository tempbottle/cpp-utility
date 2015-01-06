// Copyright 2014 ronaflx
#ifndef STRINGS_STRING_PIECE_H_
#define STRINGS_STRING_PIECE_H_

#include <stddef.h>

#include <algorithm>
#include <cstring>
#include <string>
#include <iterator>
#include <memory>

class StringPiece {
 public:
  // standard STL container boilerplate
  typedef size_t size_type;
  typedef char value_type;
  typedef const value_type* pointer;
  typedef const value_type& reference;
  typedef const value_type& const_reference;
  typedef ptrdiff_t difference_type;
  typedef const value_type* const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  static const size_type npos;

  StringPiece() : ptr_(nullptr), length_(0) {}

  StringPiece(const char* str) : ptr_(str), length_(strlen(str)) {}  // NOLINT

  StringPiece(const char* str, size_type size) : ptr_(str), length_(size) {}

  StringPiece(StringPiece x, size_type pos)  // NOLINT
      : ptr_(x.ptr_ + pos), length_(x.length_ - pos) {}

  StringPiece(StringPiece x, size_type pos, size_type len)
      : ptr_(x.ptr_ + pos), length_(std::min(len, x.length_ - pos)) {}

  StringPiece(const std::string& str)
      : ptr_(str.c_str()), length_(str.size()) {}

  const char* data() const { return ptr_; }

  size_type size() const { return length_; }

  size_type length() const { return length_; }

  bool empty() const { return length_ == 0; }

  void clear() {
    ptr_ = nullptr;
    length_ = 0;
  }

  void set(const char* str) {
    ptr_ = str;
    length_ = strlen(str);
  }

  void set(const char* str, size_type len) {
    ptr_ = str;
    length_ = len;
  }

  void set(const void* data, size_type len) {
    ptr_ = reinterpret_cast<const char*>(data);
    length_ = len;
  }

  char operator[](size_type i) const { return ptr_[i]; }

  void remove_prefix(size_type n) {
    ptr_ += n;
    length_ -= n;
  }

  void remove_suffix(size_type n) { length_ -= n; }

  std::string ToString() const {
    if (ptr_ == NULL) return std::string();
    return std::string(data(), size());
  }

  bool starts_with(StringPiece x) const {
    return (length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_) == 0);
  }

  bool ends_with(StringPiece x) const {
    return ((length_ >= x.length_) &&
            (memcmp(ptr_ + (length_ - x.length_), x.ptr_, x.length_) == 0));
  }

  // Iterator definition
  const_iterator begin() const { return ptr_; }

  const_iterator end() const { return ptr_ + length_; }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(ptr_ + length_);
  }

  const_reverse_iterator rend() const { return const_reverse_iterator(ptr_); }

  // String-like fucntion.
  std::string substr(size_type pos, size_type n = npos) const;
  size_type copy(char* dest, size_type count, size_type pos = 0) const;
  void swap(StringPiece& other);

 private:
  const char* ptr_;
  size_type length_;
};

bool operator==(const StringPiece& x, const StringPiece& y);

bool operator!=(const StringPiece& x, const StringPiece& y);

bool operator<(const StringPiece& x, const StringPiece& y);

bool operator>(const StringPiece& x, const StringPiece& y);

bool operator<=(const StringPiece& x, const StringPiece& y);
bool operator>=(const StringPiece& x, const StringPiece& y);

std::ostream& operator<<(std::ostream& stream, const StringPiece& s);

#endif  // STRINGS_STRING_PIECE_H_
