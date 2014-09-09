// Copyright 2014 ronaflx
#include <gtest/gtest.h>
#include "cpp-utility-config.h"

// #include "base/clock.h"
// #include "strings/string_piece_test.h"
// #include "strings/stringprintf_test.h"
// #include "util/singleton_test.h"
// #include "util/registerer_test.h"
// #include "util/csv/csv_test.h"
// #include "util/comparator_test.h"

#include <cstdio>
int main(int argc, char* argv[]) {
  printf("%d\n", CPP_UTILITY_VERSION_MAJOR);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
