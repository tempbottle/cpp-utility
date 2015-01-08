#include "util/auto_cleaner.h"

#include <gtest/gtest.h>
#include <string>
#include <functional>
#include <atomic>
#include <thread>
#include <vector>

namespace {
int gValue = 1;
void func() {
  gValue = 2;
}
}  // namespace

TEST(AutoCleanerTest, BasicFunction) {
  { AutoCleaner<void (*)()> value_changer = func; }
  EXPECT_EQ(2, gValue);
}

TEST(AutoCleanerTest, StdFunction) {
  std::string value = "unrelease";
  {
    std::function<void()> cleaner_function = [&value]()->void {
      value = "release";
    };
    AutoCleaner<std::function<void()>> cleaner = cleaner_function;
  }
  EXPECT_EQ("release", value);
}

TEST(AutoCleanerTest, LambdaFunction) {
  std::string value = "unrelease";
  {
    auto cleaner_func = [&value]()->void { value = "release"; };
    typedef decltype(cleaner_func) LambdaType;
    AutoCleaner<LambdaType> cleaner = cleaner_func;
  }
  EXPECT_EQ("release", value);
}

int main (int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
