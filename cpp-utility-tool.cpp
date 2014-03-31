// Copyright 2014 ronaflx
#include <gtest/gtest.h>

#include "strings/string_piece_test.h"
#include "strings/stringprintf_test.h"
#include "util/singleton_test.h"
#include "util/registerer_test.h"
#include "util/csv/csv_test.h"
#include "util/comparator_test.h"

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  return 0;
}
