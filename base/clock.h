// Copyright 2014 ronaflx
#ifndef BASE_CLOCK_H_
#define BASE_CLOCK_H_

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <mutex>

#include "base/integral_types.h"
#include "base/sysinfo.h"
class CPUClock {
 public:
  explicit CPUClock()
      : time_(NowInMicros()),
        back_ground_thread_(&CPUClock::BackGroundThread, this),
        back_ground_mutex_(),
        back_ground_cond_(),
        done_(false) {
  }

  int64 NowInMicros() const {
    const double t = GetChildrenCPUUsage() + GetCPUUsage();
    return static_cast<int64>(t * 1000000);
  }

  bool Reach(int64 target_micros) const {
    return std::atomic_load(&time_) >= target_micros;
  }

  ~CPUClock() {
    {
      std::unique_lock<std::mutex> lock(back_ground_mutex_);
      done_ = true;
      back_ground_cond_.notify_one();
    }
    back_ground_thread_.join();
  }
 private:
  void BackGroundThread() {
    std::unique_lock<std::mutex> lock(back_ground_mutex_);
    while (!done_) {
      back_ground_cond_.wait_for(lock, std::chrono::microseconds(100));
      std::atomic_store(&time_, NowInMicros());
    }
  }
  std::atomic<int64> time_;
  std::thread back_ground_thread_;
  std::mutex back_ground_mutex_;
  std::condition_variable back_ground_cond_;
  bool done_;
};
#endif  // BASE_CLOCK_H_
