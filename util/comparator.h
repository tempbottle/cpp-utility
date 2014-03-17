#ifndef UTIL_COMPARATOR_H_
#define UTIL_COMPARATOR_H_
#include <functional>
#include <tuple>

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

template<class Functor, class Compare = std::less<typename Functor::type_value> >
class OrderBy {
 public:
  OrderBy(Functor functor, Compare compare)
      : functor_(functor),
        compare_(compare) {
  }
  OrderBy(Functor functor)
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

template<class ... C>
class ChainComparators {
 public:
  explicit ChainComparators(const C& ... c)
      : c_(std::make_tuple(c...)) {
  }

  template<class T>
  bool operator()(const T& a, const T& b) const {
    return false;
  }

  template<class T>
  bool CompareWith(const T&a, const T& b, int tag) const {
    if (tag == sizeof...(C)) {
      return false;
    }
    if (std::get<tag>(c_)(a, b)) return true;
    if (std::get<tag>(c_)(b, a)) return false;
    return CompareWith(a, b, tag + 1);
  }
private:
  std::tuple<C...> c_;
};

#endif  // UTIL_COMPARATOR_H_
