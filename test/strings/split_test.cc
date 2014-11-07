// Copyright 2014 ronaflx

#include <gmock/gmock.h>

#include "strings/split.h"
#include "strings/string_piece.h"

TEST(StringSplit, SplitByCharacterNormal) {
  std::string const test_string = "a b c";
  std::vector<StringPiece> actual = Split(test_string, ' ');
  ASSERT_EQ(3, actual.size());
  EXPECT_THAT(actual, ::testing::Contains("a"));
  EXPECT_THAT(actual, ::testing::Contains("b"));
  EXPECT_THAT(actual, ::testing::Contains("c"));
}

TEST(StringSplit, SplitByStringNormal) {
  std::string const sentence = "My name is  String.";
  std::vector<StringPiece> actual = Split(sentence, " ");
  ASSERT_EQ(5, actual.size());
  EXPECT_THAT(actual, ::testing::Contains("My"));
  EXPECT_THAT(actual, ::testing::Contains("name"));
  EXPECT_THAT(actual, ::testing::Contains("is"));
  EXPECT_THAT(actual, ::testing::Contains(""));
  EXPECT_THAT(actual, ::testing::Contains("String."));
}

TEST(StringSplit, SkipEmpty) {
  std::string const sentence = "This is  string split  function";
  std::vector<StringPiece> actual = Split(sentence, " ", SkipEmpty());
  ASSERT_EQ(5, actual.size());
  EXPECT_THAT(actual, ::testing::Contains("This"));
  EXPECT_THAT(actual, ::testing::Contains("is"));
  EXPECT_THAT(actual, ::testing::Contains("string"));
  EXPECT_THAT(actual, ::testing::Contains("split"));
  EXPECT_THAT(actual, ::testing::Contains("function"));
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
