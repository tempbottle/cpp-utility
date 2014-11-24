#include <codecvt>
#include <limits>
#include <locale>
#include <gmock/gmock.h>

#include "strings/encoding.h"

TEST(Encoding, UTF8EncodeBad) {
  // UTF8 code point is in range[0x0, 0x1FFFF]
  std::string encoded;
  ASSERT_FALSE(EncodeUTF8(-1, &encoded));
  ASSERT_FALSE(EncodeUTF8(0x200000, &encoded));
}

// RFC 3629: Not all unicode value [U+0000 through U+1FFFFF] are legal values.
TEST(Encoding, UTF8EncodeGood) {
  // TODO(ronaflx): Learning C++ Localizations library.
  typedef std::codecvt_utf8<char32_t> UTF8Converter;
  std::string encoded, utf8;
  std::wstring_convert<UTF8Converter, char32_t> converter;
  for (int64 code_point = 0x00; code_point < (1 << 21); code_point++) {
    // Skip illegal unicode values.
    try {
      utf8 = converter.to_bytes(code_point);
      encoded.clear();
      ASSERT_TRUE(EncodeUTF8(code_point, &encoded));
      ASSERT_EQ(utf8, encoded);
    }
    catch (std::range_error& e) {
    }
  }
}

TEST(Encoding, UTF8DecodeGood) {
  std::string utf8;
  for (int64 code_point = 0x00; code_point < (1 << 21); code_point++) {
    utf8.clear();
    ASSERT_TRUE(EncodeUTF8(code_point, &utf8));
    ASSERT_EQ(code_point, DecodeUTF8(utf8));
  }
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
