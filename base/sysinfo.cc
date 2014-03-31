// Copyright 2014 ronaflx
#include "base/sysinfo.h"

#if defined OS_MACOSX
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#include <sys/sysctl.h>
#include <unistd.h>

#include <boost/thread/once.hpp>
#include <boost/thread/thread.hpp>
#include <cstdio>
#include "base/macros.h"
#include "base/cycleclock.h"

static boost::once_flag cpuinfo_init = BOOST_ONCE_INIT;

static double cpuinfo_cycles_per_second = 1.0;
static double cpuinfo_time_base_frequency = 1.0;
static int cpuinfo_num_cpus = 1;

static void InitializeSystemInfo() {
#if defined OS_MACOSX
  mach_timebase_info_data_t timebase_info;
  mach_timebase_info(&timebase_info);
  double mach_time_units_per_nanosecond = static_cast<double>(timebase_info
      .denom) / static_cast<double>(timebase_info.numer);
  cpuinfo_cycles_per_second = mach_time_units_per_nanosecond * 1e9;
  cpuinfo_time_base_frequency = cpuinfo_cycles_per_second;

  int num_cpus = 0;
  size_t size = sizeof(num_cpus);
  int numcpus_name[] = { CTL_HW, HW_NCPU };
  if (::sysctl(numcpus_name, arraysize(numcpus_name), &num_cpus, &size, 0, 0)
      == 0 && (size == sizeof(num_cpus)))
    cpuinfo_num_cpus = num_cpus;
#else
  // Generic cycles per second counter
#endif
}

double NominalCPUFrequency() {
  boost::call_once(cpuinfo_init, InitializeSystemInfo);
  return cpuinfo_cycles_per_second;
}

int NumCPUs() {
  boost::call_once(cpuinfo_init, InitializeSystemInfo);
  return cpuinfo_num_cpus;
}

double CycleClockFrequency() {
  boost::call_once(cpuinfo_init, InitializeSystemInfo);
  return cpuinfo_time_base_frequency;
}
