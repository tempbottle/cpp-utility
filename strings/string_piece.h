#ifndef STRINGS_STRINGPIECE_H_
#define STRINGS_STRINGPIECE_H_

#include <algorithm>
#include <stddef.h>
#include <string>
#include <iterator>
#include <memory>

template<typename T>
class StringPieceDetail {
 public:
  // standard STL container boilerplate
  typedef size_t size_type;
  typedef T value_type;
  typedef const value_type* pointer;
  typedef const value_type& reference;
  typedef const value_type& const_reference;
  typedef ptrdiff_t difference_type;
  typedef const value_type* const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  static const size_type npos;

  StringPieceDetail()
      : ptr_(nullptr),
        length_(0) {
  }

  explicit StringPieceDetail(const T* str)
      : ptr_(str),
        length_(strlen(str)) {
  }

  StringPieceDetail(const T* str, size_type size)
      : ptr_(str),
        length_(size) {
  }

  StringPieceDetail(StringPieceDetail x, size_type pos)
      : ptr_(x.ptr_ + pos),
        length_(x.length_ - pos) {

  }

  StringPieceDetail(StringPieceDetail x, size_type pos, size_type len)
      : ptr_(x.ptr_ + pos),
        length_(std::min(len, x.length_ - pos)) {

  }

  template<class Allocator>
  StringPieceDetail(
      const std::basic_string<T, std::char_traits<T>, Allocator>& str)
      : ptr_(str.c_str()),
        length_(str.size()) {
  }

  const T* data() const {
    return ptr_;
  }

  size_type size() const {
    return length_;
  }

  size_type length() const {
    return length_;
  }

  bool empty() const {
    return length_ == 0;
  }

  void clear() {
    ptr_ = nullptr;
  }

  void set(const T* str) {
    ptr_ = str;
    length_ = strlen(str);
  }

  void set(const T* str, size_type len) {
    ptr_ = str;
    length_ = len;
  }

  void set(const void* data, size_type len) {
    ptr_ = reinterpret_cast<const char*>(data);
    length_ = len;
  }

  T operator[](size_type i) const {
    return ptr_[i];
  }

  void remove_prefix(size_type n) {
    ptr_ += n;
    length_ -= n;
  }

  void remove_suffix(size_type n) {
    length_ -= n;
  }

  template<class Allocator = std::allocator<T>>
  std::basic_string<T, std::char_traits<T>, Allocator> ToString() const {
    if (ptr_ == NULL)
      return std::basic_string<T, std::char_traits<T>, Allocator>();
    return std::basic_string<T, std::char_traits<T>, Allocator>(data(), size());
  }

  bool starts_with(StringPieceDetail x) const {
    return (length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_) == 0);
  }

  bool ends_with(StringPieceDetail x) const {
    return ((length_ >= x.length_)
        && (memcmp(ptr_ + (length_ - x.length_), x.ptr_, x.length_) == 0));
  }

  // Iterator definition
  const_iterator begin() const {
    return ptr_;
  }

  const_iterator end() const {
    return ptr_ + length_;
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(ptr_ + length_);
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(ptr_);
  }
 private:
  const T* ptr_;
  size_type length_;
};

template<typename T>
inline bool operator ==(const StringPieceDetail<T>& x,
                        const StringPieceDetail<T>& y) {
  typedef typename StringPieceDetail<T>::size_type size_type;
  size_type x_length = x.size();
  size_type y_length = y.size();
  return x_length == y_length && !memcmp(x.data(), y.data(), x_length);
}

template<typename T>
inline bool operator !=(const StringPieceDetail<T>& x,
                        const StringPieceDetail<T>& y) {
  return !(x == y);
}

template<typename T>
inline bool operator <(const StringPieceDetail<T>& x,
                       const StringPieceDetail<T>& y) {
  typedef typename StringPieceDetail<T>::size_type size_type;
  size_type x_length = x.size();
  size_type y_length = y.size();
  size_type length = std::min(x_length, y_length);
  const int r = memcmp(x.data(), y.data(), length);
  return r < 0 || (r == 0 && x_length < y_length);
}

template<typename T>
inline bool operator >(const StringPieceDetail<T>& x,
                       const StringPieceDetail<T>& y) {
  return y < x;
}

template<typename T>
inline bool operator <=(const StringPieceDetail<T>& x,
                        const StringPieceDetail<T>& y) {
  return !(y < x);
}

template<typename T>
inline bool operator >=(const StringPieceDetail<T>& x,
                        const StringPieceDetail<T>& y) {
  return !(x < y);
}

template<typename T>
const typename StringPieceDetail<T>::size_type StringPieceDetail<T>::npos = -1;

typedef StringPieceDetail<char> StringPiece;
typedef StringPieceDetail<wchar_t> WStringPiece;
#endif /* STRINGS_STRINGPIECE_H_ */
