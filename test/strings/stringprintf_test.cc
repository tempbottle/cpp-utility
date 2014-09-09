// Copyright 2014 ronaflx
#include "strings/stringprintf.h"
#include <gtest/gtest.h>

#include <string>

TEST(TestStringPrintf, SmallCase) {
  std::string output = StringPrintf("%d %d", 1, 2);
  EXPECT_EQ(output, "1 2");
  StringPrintf(&output, "%d %d", 3, 4);
  EXPECT_EQ(output, "3 4");
  StringPrintfAppend(&output, " %d %d", 1, 2);
  EXPECT_EQ(output, "3 4 1 2");
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
