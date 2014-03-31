// Copyright 2014 ronaflx
#ifndef BASE_TIMER_H_
#define BASE_TIMER_H_
#include <sys/time.h>

#include "base/walltime.h"
#include "base/macros.h"
#include "base/integral_types.h"

class TimevalData {
 public:
  inline TimevalData();
  inline TimevalData(const TimevalData &d);

  inline void Start(const timeval &tv);
  inline void Add(const timeval &tv);
  inline void Reset();
  inline double Get() const;     // time in second.
  inline int64 GetInMs() const;  // time in microsecond.

 private:
  static inline int64 TimevalToUsec(const timeval &tv) {
    return static_cast<int64>(1000000) * tv.tv_sec + tv.tv_usec;
  }
  int64 start_usec_;  // start time in microseconds.
  int64 sum_usec_;    // sum of time in microseconds.
};

inline TimevalData::TimevalData()
    : start_usec_(0),
      sum_usec_(0) {
}

inline TimevalData::TimevalData(const TimevalData &d)
    : start_usec_(d.start_usec_),
      sum_usec_(d.sum_usec_) {
}

inline void TimevalData::Start(const timeval &tv) {
  start_usec_ = TimevalToUsec(tv);
}

inline void TimevalData::Add(const timeval &tv) {
  sum_usec_ += TimevalToUsec(tv) - start_usec_;
}

inline void TimevalData::Reset() {
  start_usec_ = 0;
  sum_usec_ = 0;
}

inline double TimevalData::Get() const {
  return static_cast<double>(sum_usec_) * 1e-6;
}

inline int64 TimevalData::GetInMs() const {
  return sum_usec_ / 1000;
}

class WallTimer {
 public:
  inline WallTimer();

  inline void Start();
  inline void Stop();
  inline bool Reset();
  inline void Restart();
  inline bool IsRunning() const;
  inline double Get() const;
  inline int64 GetInMs() const;

  inline static WallTime Now();
  inline static int64 NowInMs();
  inline static int64 NowInUsec();

 private:
  TimevalData data_;
  bool has_started_;

  DISALLOW_COPY_AND_ASSIGN(WallTimer);
};

inline WallTimer::WallTimer()
    : data_(),
      has_started_(false) {
}

inline void WallTimer::Start() {  // Just save when we started
  struct timeval tv;
  gettimeofday(&tv, NULL);
  data_.Start(tv);
  has_started_ = true;
}

inline void WallTimer::Stop() {   // Update total time, 1st time it's called
  if (has_started_) {           // so two Stop()s is safe
    struct timeval tv;
    gettimeofday(&tv, NULL);
    data_.Add(tv);
    has_started_ = false;
  }
}

inline bool WallTimer::Reset() {  // As if we had hit Stop() first
  data_.Reset();
  has_started_ = false;
  return true;
}

inline void WallTimer::Restart() {
  Reset();
  Start();
}

inline bool WallTimer::IsRunning() const {
  return has_started_;
}

inline double WallTimer::Get() const {
  TimevalData data(data_);
  if (has_started_) {           // need to include current time too
    struct timeval tv;
    gettimeofday(&tv, NULL);
    data.Add(tv);
  }
  return data.Get();
}

inline int64 WallTimer::GetInMs() const {
  TimevalData data(data_);
  if (has_started_) {           // need to include current time too
    struct timeval tv;
    gettimeofday(&tv, NULL);
    data.Add(tv);
  }
  return data.GetInMs();
}

inline WallTime WallTimer::Now() {
  return WallTime_Now();
}

inline int64 WallTimer::NowInMs() {
  return static_cast<int64>(1000 * WallTime_Now());
}

inline int64 WallTimer::NowInUsec() {
  return static_cast<int64>(1000) * 1000 * WallTime_Now();
}

#endif  // BASE_TIMER_H_
