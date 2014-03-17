#ifndef STRINGS_STRINGPRINTF_TEST_H_
#define STRINGS_STRINGPRINTF_TEST_H_

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

#endif  // STRINGS_STRINGPRINTF_TEST_H_
