// Copyright 2014 ronaflx
#ifndef UTIL_STATISTICS_H_
#define UTIL_STATISTICS_H_
#include <vector>
#include <numeric>
#include <cmath>

template <typename R, typename T>
typename std::enable_if<std::is_floating_point<R>::value, R>::type Correlation(
    const std::vector<T>& x, const std::vector<T>& y) {
  const size_t n = x.size();
  if (n < 2 || n != y.size()) {
    return 0;
  }
  const R sum_x = std::accumulate(x.begin(), x.end(), R());
  const R sum_y = std::accumulate(y.begin(), y.end(), R());
  const R mean_x = sum_x / n;
  const R mean_y = sum_y / n;
  R sum_xx = 0, sum_xy = 0, sum_yy = 0;
  for (int i = 0; i < n; i++) {
    const double delta_x = x[i] - mean_x;
    const double delta_y = y[i] - mean_y;
    sum_xx += delta_x * delta_x;
    sum_xy += delta_x * delta_y;
    sum_yy += delta_y * delta_y;
  }
  const R denominator = sum_xx * sum_yy;
  if (denominator == 0) {
    return 0;
  } else {
    return sum_xy / sqrt(denominator);
  }
}

// TODO(ronaflx): Can we use template specialization to do this.
template <typename T>
typename std::enable_if<
    std::is_integral<T>::value || std::is_floating_point<T>::value, T>::type
Median(const std::vector<T>& values) {
  const size_t n = values.size();
  if (n == 0) return T();
  std::vector<T> container = values;
  typename std::vector<T>::iterator middle = container.begin() + n / 2;
  std::nth_element(container.begin(), middle, container.end());
  if (n & 1) {
    return *middle;
  } else {
    return (*middle + *max_element(container.begin(), middle)) / 2.0;
  }
}

#endif  // UTIL_STATISTICS_H_
