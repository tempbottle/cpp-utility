// Copyright 2014 ronaflx
// system info for mac for now.

#ifndef BASE_SYSINFO_H_
#define BASE_SYSINFO_H_

double NominalCPUFrequency();
int NumCPUs();
double CycleClockFrequency();

// Get CPU usage of calling process.
double GetCPUUsage();
// Get CPU usage of calling process and children. These statistics will include
// the resources used by grand-children, and further removed descendants
double GetChildrenCPUUsage();

#endif  // BASE_SYSINFO_H_
