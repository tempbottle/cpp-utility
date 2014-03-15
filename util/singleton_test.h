#ifndef SINGLETON_TEST_H_
#define SINGLETON_TEST_H_

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

#endif /* SINGLETON_TEST_H_ */
