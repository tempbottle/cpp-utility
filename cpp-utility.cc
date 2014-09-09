// Copyright 2014 ronaflx
#include <gtest/gtest.h>
#include "cpp-utility-config.h"

#include <cstdio>
int main(int argc, char* argv[]) {
  printf("%d\n", CPP_UTILITY_VERSION_MAJOR);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
