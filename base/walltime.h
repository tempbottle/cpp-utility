// Copyright 2014 ronaflx
#ifndef BASE_WALLTIME_H_
#define BASE_WALLTIME_H_

#include "base/integral_types.h"

typedef double WallTime;

extern WallTime WallTime_Now();
extern int64 WallTime_CPS();
extern double WallTime_SPC();
extern int WallTime_Recalibrations();

#endif  // BASE_WALLTIME_H_
