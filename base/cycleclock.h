// Copyright 2014 ronaflx
#ifndef BASE_CYCLECLOCK_H_
#define BASE_CYCLECLOCK_H_

#ifdef OS_MACOSX
#include <mach/mach_time.h>
#endif

#include "base/integral_types.h"
#include "base/sysinfo.h"

class CycleClock {
 public:
  static inline int64 Now();
  static double Frequency();

 private:
  CycleClock();  // no instances
};

#ifdef OS_MACOSX
inline int64 CycleClock::Now() {
  return mach_absolute_time();
}
#endif

#endif  // BASE_CYCLECLOCK_H_
