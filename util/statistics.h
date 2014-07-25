// Copyright 2014 ronaflx
#ifndef UTIL_STATISTICS_H_
#define UTIL_STATISTICS_H_
#include <vector>
using std::vector;

template<typename T, typename R>
R Correlation(const vector<T>& x, const vector<T>& y);

template<typename R>
R Median(const vector<R>& values);

#endif  // UTIL_STATISTICS_H_
