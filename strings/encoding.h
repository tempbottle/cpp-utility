#ifndef STRINGS_ENCODING_H_
#define STRINGS_ENCODING_H_

#include <string>
#include <cstdlib>

#include "base/integral_types.h"


// *** Unicode related ***
// Encode a int64 unicode to std::string as UTF8 format.
// Return true if the code is in [U+0000, U+1FFFFF].
// The UTF8 contains at most 8 bytes. Suppose the they're 1-base index.
// Byte-1 is the most significant bit of code point.
bool EncodeUTF8(int64 code, std::string* out);

// Decode an UTF8 format sequence to int64.
// Return -1 if the sequence is a invalid UTF8 code.
int64 DecodeUTF8(const char* seq, size_t len);
int64 DecodeUTF8(const std::string& seq);
// Decode an UTF8 stream to a code point.
// Return the number character comsume or -1 if the sequence is invalid.
int DecodeUTF8CodePoint(const char* seq, size_t len, int64* code_point);

#endif  // STRINGS_ENCODING_H_
