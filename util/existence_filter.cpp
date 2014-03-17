#include "util/existence_filter.h"

#include <boost/scoped_ptr.hpp>

#include <cstring>
#include <cmath>

#include "base/macros.h"

class ExistenceFilter::BlockBitmap {
 public:
  explicit BlockBitmap(uint32 length);
  virtual ~BlockBitmap();

  size_t Size() const;
  void Clear();
  bool Get(uint32 index) const;
  void Set(uint32 index);
  bool CopyFragmentTo(uint32* iter, char* buf, int* bytes);
  bool CopyFragmentFrom(uint32* iter, const char* buf, int* bytes);

 private:
  static const int kBlockShift = 21;  // 2^21 bits == 256KB block
  static const int kBlockBits = 1 << kBlockShift;
  static const int kBlockBytes = kBlockBits >> 3;  // kBlockBits / 8
  static const int kBlockWords = kBlockBits >> 5;  // kBlockBits / 32
  static const int kBlockMask = kBlockBits - 1;

  // Array of blocks
  uint32 **block_;
  const uint32 length_;
  const uint32 num_blocks_;
  const uint32 bytes_in_last_;

  uint32 NumBlock(uint32 length) {
    return (length >> kBlockShift) + ((length & kBlockMask) != 0);
  }
  uint32 NumBytesInLastBlock(uint32 length) {
    int remind = length & kBlockMask;
    return remind == 0 ? kBlockBytes : (remind + 31) / 32 * sizeof(**block_);
  }

  DISALLOW_COPY_AND_ASSIGN(BlockBitmap);
};

ExistenceFilter::BlockBitmap::BlockBitmap(uint32 length)
    : length_(length),
      num_blocks_(NumBlock(length)),
      bytes_in_last_(NumBytesInLastBlock(length)) {
  block_ = new uint32*[num_blocks_];
  for (int i = 0; i < num_blocks_ - 1; i++) {
    block_[i] = new uint32[kBlockWords];
  }
  block_[num_blocks_ - 1] = new uint32[bytes_in_last_ / sizeof(**block_)];
}

ExistenceFilter::BlockBitmap::~BlockBitmap() {
  for (int i = 0; i < num_blocks_; i++) {
    delete[] block_[i];
  }
  delete[] block_;
}

size_t ExistenceFilter::BlockBitmap::Size() const {
  return bytes_in_last_ + (num_blocks_ - 1) * kBlockBytes;
}

void ExistenceFilter::BlockBitmap::Clear() {
  for (int i = 0; i < num_blocks_; i++) {
    memset(block_[i], 0, kBlockBytes);
  }
  memset(block_[num_blocks_ - 1], 0, bytes_in_last_);
}

bool ExistenceFilter::BlockBitmap::Get(uint32 index) const {
  const int bindex = index >> kBlockShift;
  const int windex = index & kBlockMask;
  const uint32 bitmask = 1u << (index & 31);
  return (block_[bindex][windex] & bitmask) != 0;
}

void ExistenceFilter::BlockBitmap::Set(uint32 index) {
  const int bindex = index >> kBlockShift;
  const int windex = index & kBlockMask;
  const uint32 bitmask = 1u << (index & 31);
  block_[bindex][windex] |= bitmask;
}

bool ExistenceFilter::BlockBitmap::CopyFragmentTo(uint32* iter, char* buf,
                                                  int* bytes) {
  if (*iter >= num_blocks_) {
    return false;
  }
  *bytes = (*iter == num_blocks_ - 1 ? bytes_in_last_ : kBlockBytes);
  memcpy(buf, block_[*iter], *bytes);
  (*iter)++;
  return true;
}

bool ExistenceFilter::BlockBitmap::CopyFragmentFrom(uint32* iter,
                                                    const char* buf,
                                                    int* bytes) {
  if (*iter >= num_blocks_) {
    return false;
  }
  *bytes = (*iter == num_blocks_ - 1 ? bytes_in_last_ : kBlockBytes);
  memcpy(block_[*iter], buf, *bytes);
  (*iter)++;
  return true;
}

ExistenceFilter::ExistenceFilter(uint32 m, uint32 n, int k)
    : vec_size_(m ? m : 1),
      expected_nelts_(n),
      num_hashes_(k) {
  rep_.reset(new BlockBitmap(vec_size_));
  rep_->Clear();
}

/* static */
ExistenceFilter* ExistenceFilter::CreateOptimal(size_t size_in_bytes,
                                                uint32 estimated_insertions) {
  const uint32 m = size_in_bytes * 8;
  const uint32 n = estimated_insertions;

  int optimal_k = static_cast<int>(
      (static_cast<float>(m) / n * log(2.0)) + 0.5);
  if (optimal_k < 1) {
    optimal_k = 1;
  }
  if (optimal_k > 7) {
    optimal_k = 7;
  }
  return new ExistenceFilter(m, n, optimal_k);
}

ExistenceFilter::~ExistenceFilter() {
}

void ExistenceFilter::Clear() {
}

bool ExistenceFilter::Exists(uint64 hash) const {
  for (int i = 0; i < num_hashes_; i++) {
    if (!rep_->Get(hash % vec_size_)) {
      return false;
    }
    hash = RotateLeft64(hash, 8);
  }
  return true;
}

void ExistenceFilter::Insert(uint64 hash) {
  for (int i = 0; i < num_hashes_; i++) {
    rep_->Set(hash % vec_size_);
    hash = RotateLeft64(hash, 8);
  }
}

size_t ExistenceFilter::Size() const {
  return BitsToWords(vec_size_) * sizeof(vec_size_);
}

size_t ExistenceFilter::MinFilterSizeInBytesForErrorRate(float error_rate,
                                                         size_t num_elements) {
  // (-num_hashes * num_elements) / log(1 - error_rate^(1/num_hashes))

  double min_bits = 0;
  for (size_t num_hashes = 1; num_hashes < 8; ++num_hashes) {
    double num_bits = (-1.0 * num_hashes * num_elements)
        / log(1.0 - pow(static_cast<double>(error_rate), (1.0 / num_hashes)));
    if (min_bits == 0 || num_bits < min_bits)
      min_bits = num_bits;
  }
  return static_cast<size_t>(ceil(min_bits / 8));
}

/* static */
uint64 ExistenceFilter::RotateLeft64(uint64 original, int num_bits) {
  int num_bot_bits = num_bits;
  uint64 mask_bits = (1 << num_bot_bits) - 1;
  uint64 old_bot_part = original & mask_bits;
  uint64 new_bot_part = original >> num_bot_bits;
  uint64 new_top_part = old_bot_part << (64 - num_bot_bits);
  uint64 rotated_original = new_bot_part | new_top_part;
  return rotated_original;
}

int ExistenceFilter::Write(char** buf, size_t* size) {
  const int require_bytes = sizeof(Header) + Size();

  *buf = new char[require_bytes];
  if (*buf == NULL) {
    return -1;
  }
  *size = require_bytes;

  char *buf_ptr = *buf;

  // write header
  Header header = { vec_size_, expected_nelts_, num_hashes_ };
  memcpy(buf_ptr, &header, sizeof(Header));
  buf_ptr += sizeof(Header);

  // write bitmap
  int bytes = 0;
  int write = 0;
  for (uint32 iter = 0; rep_->CopyFragmentTo(&iter, buf_ptr, &bytes);) {
    buf_ptr += bytes;
    write += bytes;
  }
  return write;
}

/* static */
bool ExistenceFilter::ReadHeader(const char* buf, Header* header) {
  memcpy(header, buf, sizeof(Header));
  if (header->k >= 8 || header->k <= 0) {
    return false;
  }
  return true;
}

/* static */
ExistenceFilter* ExistenceFilter::Read(const char* buf, size_t size) {
  Header header;
  ReadHeader(buf, &header);
  buf += sizeof(Header);
  const uint32 filter_size = BitsToWords(header.m);
  const uint32 filter_bytes = filter_size * sizeof(filter_size);
  if (size < sizeof(Header) + filter_bytes) {
    return NULL;
  }
  ExistenceFilter* filter = new ExistenceFilter(header.m, header.n, header.k);
  int bytes = 0;
  for (uint32 iter = 0; filter->rep_->CopyFragmentFrom(&iter, buf, &bytes);) {
    buf += bytes;
  }
  return filter;
}
