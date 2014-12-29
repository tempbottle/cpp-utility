// Copyright 2014 ronaflx
// Description:
// This class is used to build global/class static variable for non-POD type.
// When the main function ends, the program will destruct all global non-POD
// variable while the other threads maybe still running. If the thread access
// the global variable, program will crush.

// Solution:
// This class will hold the variable in heap and never destruct it.
// The variable will be initialized during first access. All the function in is
// class is immutable, so all the member of this class will use keyword mutable.
// This doesn't hurt anything, since the variable will only be modified once
// when initializing and there is a once_flag to protect this action.

// Note:
// Please make sure the args passed to constructor are POD types.
// We will store a copy of ags inside of this class.
// This class is not thread-safe except for initialization.

#include <functional>
#include <mutex>
#include <tuple>

template <typename Type, typename... Args>
class LazyPtr {
 public:
  typedef Type value_type;
  LazyPtr(const Args&... args)
      : args_(std::make_tuple(args...)), ptr_(nullptr) {}

  Type& operator*() const { return *get(); }
  Type* operator->() const { return get(); }

  Type* get() const {
    std::call_once(module_init_, std::bind(&LazyPtr::InitModule, this));
    return ptr_;
  }

 private:
  template <int...>
  struct seq {};

  template <int N, int... S>
  struct gens : gens<N - 1, N - 1, S...> {};
  template <int... S>
  struct gens<0, S...> {
    typedef seq<S...> type;
  };

  void InitModule() const {
    InitModuleInternal(typename gens<sizeof...(Args)>::type());
  }

  template <int... S>
  void InitModuleInternal(seq<S...>) const {
    ptr_ = new Type(std::get<S>(args_)...);
  }

  std::tuple<Args...> args_;
  mutable Type* ptr_;
  mutable std::once_flag module_init_;

  // Disallow copy constructor.
  LazyPtr(const LazyPtr&);
};
