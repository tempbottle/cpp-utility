#include "strings/encoding.h"

bool EncodeUTF8(int64 code_point, std::string* out) {
  if (code_point < 0 || code_point >= 0x200000) {
    return false;
  } else if (code_point < 0x80) {
    // 7 bits of code_point point.
    *out += code_point;
  } else if (code_point < 0x800) {
    // 11 bits of code_point point.
    *out += (code_point >> 6) | 0xC0;      // 110xxxxx
    *out += ((code_point) & 0x3F) | 0x80;  // 10xxxxxx
  } else if (code_point < 0x10000) {
    // 16 bits of code_point point.
    *out += (code_point >> 12) | 0xE0;          // 1110xxxx
    *out += ((code_point >> 6) & 0x3F) | 0x80;  // 10xxxxxx
    *out += (code_point & 0x3F) | 0x80;         // 10xxxxxx
  } else {
    // 21 bits of code_point point.
    *out += (code_point >> 18) | 0xF0;           // 11110xxx
    *out += ((code_point >> 12) & 0x3F) | 0x80;  // 10xxxxxx
    *out += ((code_point >> 6) & 0x3F) | 0x80;   // 10xxxxxx
    *out += (code_point & 0x3F) | 0x80;          // 10xxxxxx
  }
  return true;
}

int64 DecodeUTF8(const char* seq, size_t len) {
  static const char pfx0 = 0xC0, msb0 = 0x80;
  static const char pfx1 = 0xE0, msb1 = 0xC0;
  static const char pfx2 = 0xF0, msb2 = 0xE0;
  static const char pfx3 = 0xF8, msb3 = 0xF0;
  int64 code_point = 0;
  if (len == 1) {
    if ((seq[0] & 0x80) != 0x00) return -1;
    code_point += seq[0];
  } else if (len == 2) {
    if ((seq[0] & pfx1) != msb1) return -1;
    if ((seq[1] & pfx0) != msb0) return -1;
    code_point = (static_cast<int64>(seq[0] ^ msb1) << 6) | (seq[1] ^ msb0);
  } else if (len == 3) {
    if ((seq[0] & pfx2) != msb2) return -1;
    if ((seq[1] & pfx0) != msb0) return -1;
    if ((seq[2] & pfx0) != msb0) return -1;
    code_point = (static_cast<int64>(seq[0] ^ msb2) << 12) |
                 (static_cast<int64>(seq[1] ^ msb0) << 6) | (seq[2] ^ msb0);
  } else if (len == 4) {
    if ((seq[0] & pfx3) != msb3) return -1;
    if ((seq[1] & pfx0) != msb0) return -1;
    if ((seq[2] & pfx0) != msb0) return -1;
    if ((seq[3] & pfx0) != msb0) return -1;
    code_point = (static_cast<int64>(seq[0] ^ msb3) << 18) |
                 (static_cast<int64>(seq[1] ^ msb0) << 12) |
                 (static_cast<int64>(seq[2] ^ msb0) << 6) | (seq[3] ^ msb0);
  } else {
    return -1;
  }
  return code_point;
}

int64 DecodeUTF8(const std::string& seq) {
  return DecodeUTF8(seq.data(), seq.size());
}
