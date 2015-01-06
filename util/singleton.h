// Copyright 2014 ronaflx
#ifndef UTIL_SINGLETON_H_
#define UTIL_SINGLETON_H_

#include <mutex>

template<typename Type>
class Singleton {
 public:
  static Type* Get() {
    std::call_once(module_init_, InitSingleton);
    return instance_;
  }

  static const Type& GetRef() {
    std::call_once(module_init_, InitSingleton);
    return *instance_;
  }

 private:
  static void InitSingleton() {
    instance_ = ::new Type();
  }

  static Type* instance_;
  static std::once_flag module_init_;
};

template<typename Type>
std::once_flag Singleton<Type>::module_init_;

template<typename Type>
Type* Singleton<Type>::instance_ = NULL;

#endif  // UTIL_SINGLETON_H_
