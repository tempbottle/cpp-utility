// Copyright 2014 ronaflx
// A implementation of copy on write object by using std::atomic

#ifndef UTIL_COPY_ON_WRITE_H_
#define UTIL_COPY_ON_WRITE_H_

#include <atomic>
#include <memory>

template <typename T>
class CopyOnWrite {
 public:
  CopyOnWrite();
  CopyOnWrite(const CopyOnWrite&);
  ~CopyOnWrite();

  CopyOnWrite& operator=(const CopyOnWrite& other);
  T* get_mutable();
  const T& get() const;
  int get_ref() const;

 private:
  struct Rep {
    std::atomic<int> ref;
    T value;
  };
  Rep* rep_;
  static void UnRef(Rep* rep) {
    rep->ref--;
    if (rep->ref == 0) {
      delete rep;
    }
  }
};

template <typename T>
CopyOnWrite<T>::CopyOnWrite()
    : rep_(new Rep) {
  rep_->ref = 1;
}

template <typename T>
CopyOnWrite<T>::CopyOnWrite(const CopyOnWrite& other)
    : rep_(other.rep_) {
  rep_->ref++;
}

template <typename T>
CopyOnWrite<T>::~CopyOnWrite() {
  UnRef(rep_);
}

template <typename T>
CopyOnWrite<T>& CopyOnWrite<T>::operator=(const CopyOnWrite& other) {
  UnRef(rep_);
  rep_ = other.rep_;
  rep_->ref++;
  return *this;
}

template <typename T>
const T& CopyOnWrite<T>::get() const {
  return rep_->value;
}

template <typename T>
T* CopyOnWrite<T>::get_mutable() {
  if (rep_->ref.load() != 1) {
    Rep* copy_version = new Rep;
    copy_version->ref = 1;
    copy_version->value = rep_->value;
    UnRef(rep_);
    rep_ = copy_version;
  }
  return &rep_->value;
}

template <typename T>
int CopyOnWrite<T>::get_ref() const {
  return rep_->ref.load();
}

#endif  // UTIL_COPY_ON_WRITE_H_
