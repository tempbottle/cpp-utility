// Copyright 2014 ronaflx
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

TEST(TestStringPiece, SubstrTest) {
  StringPiece x("abcdef");

  EXPECT_EQ(x.substr(0), "abcdef");
  EXPECT_EQ(x.substr(1, 2), "bc");
  EXPECT_EQ(x.substr(0, 100), "abcdef");
}

TEST(TestStringPiece, CopyTest) {
  const char* text = "abcdef";
  StringPiece x(text);
  char buffer[1024];

  EXPECT_EQ(x.length(), x.copy(buffer, 1024));
  EXPECT_EQ(0, memcmp(buffer, text, x.length()));

  EXPECT_EQ(x.length() - 1, x.copy(buffer, 1024, 1));
  EXPECT_EQ(0, memcmp(buffer, text + 1, x.length() - 1));

  EXPECT_EQ(x.length() - 2, x.copy(buffer, x.length() - 2, 1));
  EXPECT_EQ(0, memcmp(buffer, text + 1, x.length() - 2));
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
