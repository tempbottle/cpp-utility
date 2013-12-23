#include "base/walltime.h"

#include "base/cycleclock.h"

// TODO(ronaflx): to be finished.
WallTime WallTime_Now() {
  return CycleClock::Now();
}

int64 WallTime_CPS() {
  return CycleClock::Frequency();
}

double WallTime_SPC() {
  return 1.0 / CycleClock::Frequency();
}

int WallTime_Recalibrations() {
  return 0;
}

