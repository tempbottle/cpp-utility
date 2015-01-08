// Copyright 2014 ronaflx
#ifndef UTIL_AUTO_CLEANER_
#define UTIL_AUTO_CLEANER_

#include "base/macros.h"

template<typename F>
class AutoCleaner {
 public:
  AutoCleaner(F f) : hold_(true), func_(f) {}
  AutoCleaner(AutoCleaner&& from) : hold_(from.Release()), func_(from.func_) {}
  AutoCleaner(const AutoCleaner& from)
      : hold_(from.Release()), func_(from.func_) {}

  bool IsHold() { return hold_; }

  bool Release() {
    bool ret = hold_;
    hold_ = false;
    return ret;
  }

  ~AutoCleaner() {
    if (hold_) func_();
  }

 private:
  AutoCleaner& operator=(const AutoCleaner& other) = delete;
  bool hold_;
  F func_;
};

#endif
