// Copyright 2014 ronaflx
#include <strings/string_piece.h>
#include <ostream>  // NOLINT

std::string StringPiece::substr(size_type pos, size_type n) const {
  pos = std::min(pos, length_);
  n = std::min(n, length_ - pos);
  return std::string(ptr_ + pos, n);
}

void StringPiece::swap(StringPiece& other) {
  std::swap(ptr_, other.ptr_);
  std::swap(length_, other.length_);
}

StringPiece::size_type StringPiece::copy(char* dest, size_type count,
                                         size_type pos) const {
  size_type max_length = std::min(count, length() - pos);
  memcpy(dest, ptr_ + pos, max_length);
  return max_length;
}

const typename StringPiece::size_type StringPiece::npos = -1;

bool operator==(const StringPiece& x, const StringPiece& y) {
  typedef typename StringPiece::size_type size_type;
  size_type x_length = x.size();
  size_type y_length = y.size();
  return x_length == y_length && !memcmp(x.data(), y.data(), x_length);
}

bool operator!=(const StringPiece& x, const StringPiece& y) {
  return !(x == y);
}

bool operator<(const StringPiece& x, const StringPiece& y) {
  typedef typename StringPiece::size_type size_type;
  size_type x_length = x.size();
  size_type y_length = y.size();
  size_type length = std::min(x_length, y_length);
  const int r = memcmp(x.data(), y.data(), length);
  return r < 0 || (r == 0 && x_length < y_length);
}

bool operator>(const StringPiece& x, const StringPiece& y) { return y < x; }

bool operator<=(const StringPiece& x, const StringPiece& y) { return !(y < x); }

bool operator>=(const StringPiece& x, const StringPiece& y) { return !(x < y); }

std::ostream& operator<<(std::ostream& stream, const StringPiece& s) {
  stream.write(s.data(), s.length());
  return stream;
}
