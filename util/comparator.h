// Copyright 2014 ronaflx
#ifndef UTIL_COMPARATOR_H_
#define UTIL_COMPARATOR_H_

#include <tuple>
#include <functional>

// Functor to a field of class
template<class C, class T>
class Field {
 public:
  typedef T type_value;
  typedef C class_value;
  explicit Field(T C::*member_ptr)
      : member_ptr_(member_ptr) {
  }
  const T& operator()(const C& obj) const {
    return obj.*member_ptr_;
  }

 private:
  T C::*member_ptr_;
};

// Functor to a getter function of class
template<class C, typename T>
class Getter {
 public:
  typedef T type_value;
  typedef C class_value;
  explicit Getter(T (C::*member_ptr)() const)
      : member_ptr_(member_ptr) {
  }

  T operator()(const C& obj) const {
    return (obj.*member_ptr_)();
  }

 private:
  T (C::*member_ptr_)() const;
};

template<class Functor, class Compare = std::less<typename Functor::type_value>>
class OrderBy {
 public:
  OrderBy(Functor functor, Compare compare)
      : functor_(functor),
        compare_(compare) {
  }
  explicit OrderBy(Functor functor)
      : functor_(functor),
        compare_() {
  }
  template<class T>
  bool operator()(const T& a, const T& b) const {
    return compare_(functor_(a), functor_(b));
  }

 private:
  Functor functor_;
  Compare compare_;
};

// Using default value is OK, but I prefer to use a different function
template<class Functor, class Compare>
inline OrderBy<Functor, Compare> MakeOrderBy(Functor functor, Compare compare) {
  return OrderBy<Functor, Compare>(functor, compare);
}

template<class Functor>
inline OrderBy<Functor> MakeOrderBy(Functor functor) {
  return OrderBy<Functor>(functor);
}

template<class ... C>
class ChainComparators {
 public:
  explicit ChainComparators(const C& ... c)
      : c_(std::make_tuple(c...)) {
  }

  template<class T>
  bool operator()(const T& a, const T& b) const {
    return CompareWith(a, b, Tag<0>());
  }

 private:
  template<size_t I> struct Tag {
  };

  template<typename T>
  bool CompareWith(const T& x, const T& y, Tag<sizeof...(C)>) const {
    return false;
  }
  template <typename T, size_t I>
  bool CompareWith(const T& x, const T& y, Tag<I>) const {
    if (std::get<I>(c_)(x, y)) return true;
    if (std::get<I>(c_)(y, x)) return false;
    return CompareWith(x, y, Tag<I + 1>());
  }
  // We can not use something like:
  // CompareWith(const T& x, const T& y, int tag) const;
  // std::get<I>'s template param `I` must be determined in compiling time.
  // We can not use something like:
  // CompareWith(const T& x, const T& y, Tag<int> const {
  //   if (I = sizeof ...(C))
  // syntax error.
  std::tuple<C...> c_;
};

template<class ...C>
inline ChainComparators<C...> MakeChainComparators(const C& ... c) {
  return ChainComparators<C...>(c ...);
}
#endif  // UTIL_COMPARATOR_H_
