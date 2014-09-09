// Copyright 2014 ronaflx
#include <gtest/gtest.h>

#include "util/singleton.h"

TEST(TestSingleton, SameAddressTest) {
  int* ptr0 = Singleton<int>::Get();
  int* ptr1 = Singleton<int>::Get();
  EXPECT_EQ(ptr0, ptr1);
}

TEST(TestSingleton, TestRef) {
  int* ptr0 = Singleton<int>::Get();
  const int& ref = Singleton<int>::GetRef();
  EXPECT_EQ(ptr0, &ref);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
