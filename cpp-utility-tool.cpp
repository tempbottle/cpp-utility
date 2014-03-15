#include <iostream>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
#include <gtest/gtest.h>

#include "base/macros.h"
#include "base/sysinfo.h"
#include "base/timer.h"
#include "util/csv/parser.h"

#include "strings/string_piece_test.h"
#include "strings/stringprintf_test.h"
#include "util/singleton_test.h"
#include "util/registerer_test.h"
#include "util/csv/csv_test.h"

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  return 0;
}
