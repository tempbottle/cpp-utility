#ifndef STRINGS_STRING_PIECE_TEST_H_
#define STRINGS_STRING_PIECE_TEST_H_

#include <gtest/gtest.h>
#include "strings/string_piece.h"

TEST(TestStringPiece, BasicTest) {
  static const char* ktest_string = "StringPiece Test String";
  {
    StringPiece string_piece;
    EXPECT_EQ(string_piece.data(), nullptr);
    EXPECT_EQ(string_piece.length(), 0u);
  }
  {
    StringPiece string_piece(ktest_string);
    EXPECT_EQ(string_piece.length(), strlen(ktest_string));
    EXPECT_STREQ(string_piece.data(), ktest_string);
    string_piece.remove_prefix(5);
    EXPECT_EQ(string_piece.length(), strlen(ktest_string) - 5);
    EXPECT_STREQ(string_piece.ToString().data(), "gPiece Test String");
    string_piece.remove_suffix(5);
    EXPECT_EQ(string_piece.length(), strlen(ktest_string) - 10);
    EXPECT_STREQ(string_piece.ToString().data(), "gPiece Test S");
  }
}
TEST(TestStringPiece, OperatorTest) {
  // Test operator ==
  {
    StringPiece x("abc"), y("abc");
    EXPECT_EQ(x, y);
  }
  // Test operator !=, <, >
  {
    StringPiece x("abc"), y("xyz");
    EXPECT_NE(x, y);
    EXPECT_LT(x, y);
    EXPECT_GT(y, x);
  }
  // Test operator <=, >=
  {
    StringPiece x("abc"), y("abc"), z("abd");
    EXPECT_GE(x, y);
    EXPECT_LE(y, x);
    EXPECT_LE(x, z);
    EXPECT_GE(z, x);
  }
}

#endif  // STRINGS_STRING_PIECE_TEST_H_
