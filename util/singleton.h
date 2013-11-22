#ifndef UTIL_SINGLETON_H_
#define UTIL_SINGLETON_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>

template<typename Type>
class Singleton {
 public:
  static Type* Get() {
    boost::call_once(module_init_, InitSingleton);
    return instance_;
  }

  static const Type& GetRef() {
    boost::call_once(module_init_, InitSingleton);
    return *instance_;
  }

  // This is not thread-safe.
  static void UnsafeReset() {
    delete instance_;
    instance_ = NULL;
    module_init_ = BOOST_ONCE_INIT;
  }

 public:
  static void InitSingleton() {
    instance_ = ::new Type();
  }

  static Type* instance_;
  static boost::once_flag module_init_;
};

template<typename Type>
boost::once_flag Singleton<Type>::module_init_ = BOOST_ONCE_INIT;

template<typename Type>
Type* Singleton<Type>::instance_ = NULL;

#endif  // UTIL_SINGLETON_H_
