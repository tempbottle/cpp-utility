#include <gtest/gtest.h>
#include "base/sysinfo.h"

TEST(SysInfoTest, SysInfoTestCase) {
  EXPECT_EQ(NumCPUs(), 8);
  EXPECT_EQ(CycleClockFrequency(), 1000000000);
}
int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
