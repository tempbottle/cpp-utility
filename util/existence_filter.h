// Copyright 2014 ronaflx
/*
 * existence_filter.h
 * mainly copy from chromium storage/existence_filter.h
 */
#ifndef UTIL_EXISTENCE_FILTER_H_
#define UTIL_EXISTENCE_FILTER_H_

#include <cstddef>
#include <memory>
#include "base/integral_types.h"

class ExistenceFilter {
 public:
  struct Header {
    uint32 m;
    uint32 n;
    int k;
  };

  // 'm' is the number of bits in the bit vector
  // 'n' is the number of values that will be stored
  // 'k' is the number of hash values to use per insert/lookup
  // k must be less than 8
  ExistenceFilter(uint32 m, uint32 n, int k);

  static ExistenceFilter* CreateOptimal(size_t size_in_bytes,
                                        uint32 estimated_insertions);

  ~ExistenceFilter();

  void Clear();
  // Inserts a hash value into the filter
  // We generate 'k' separate internal hash values
  void Insert(uint64 hash);

  // Checks if the given 'hash' was previously inserted in the filter
  // It may return some false positives
  bool Exists(uint64 hash) const;

  // Returns the size (in bytes) of the bloom filter
  size_t Size() const;

  // Returns the minimum required size of the filter in bytes
  // under the given error rate and number of elements
  static size_t MinFilterSizeInBytesForErrorRate(float error_rate,
                                                 size_t num_elements);

  // return non-header size written
  // or -1 if error occurs.
  int Write(char **buf, size_t *size);

  static bool ReadHeader(const char *buf, Header* header);

  // Create a new ExistenceFilter from buf. This function will
  // not share buf's memory with the new ExistenceFilter.
  // in other words, this function create a complete ExistenceFilter
  // with its own memory.
  static ExistenceFilter* Read(const char *buf, size_t size);

 private:
  class BlockBitmap;

  // Rotate the value in 'original' by 'num_bits'
  static uint64 RotateLeft64(uint64 original, int num_bits);

  static inline uint32 BitsToWords(uint32 bits) {
    uint32 words = (bits + 31) >> 5;
    if (bits > 0 && words == 0) {
      words = 1 << (32 - 5);  // possible overflow
    }
    return words;
  }

  std::unique_ptr<BlockBitmap> rep_;  // points to bitmap
  const uint32 vec_size_;  // size of bitmap (in bits)
  const uint32 expected_nelts_;  // expected number of inserts
  const int32 num_hashes_;  // number of hashes per lookup
};

#endif  // UTIL_EXISTENCE_FILTER_H_
