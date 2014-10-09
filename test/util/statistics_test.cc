// Copyright 2014 ronaflx
#include <vector>
#include <gtest/gtest.h>
using std::vector;

#include "util/statistics.h"

TEST(MedianTest, EvenUnsorted) {
  vector<float> values = {0.0, -1.0, 1.0, 4.0, 5.0, 2.0};
  float res = Median<float>(values);
  EXPECT_FLOAT_EQ(1.5, res);
}

TEST(MedianTest, OddUnsorted) {
  vector<float> values = {0.0, -1.0, 1.0, 4.0, 2.0};
  float res = Median(values);
  EXPECT_FLOAT_EQ(1.0, res);
}

TEST(MedianTest, Evensorted) {
  vector<float> values = {-1.0, 0.0, 1.0, 2.0, 4.0, 5.0};
  float res = Median<float>(values);
  EXPECT_FLOAT_EQ(1.5, res);
}

TEST(MedianTest, IntegerType) {
  vector<int> values = {0, -1, 1, 4, 5, 2};
  int res = Median(values);
  EXPECT_FLOAT_EQ(1, res);
}

TEST(CorrelationTest, ZeroSize) {
  vector<int> x, y;
  float res = Correlation<float>(x, y);
  EXPECT_EQ(0, res);
}

TEST(CorrelationTest, IntReturnType) {
  vector<int> x = {1, 2};
  vector<int> y = {1, 3};
  // ((-0.5 * -1) + (0.5 * 1)) / ((((-0.5)^2 + 0.5^2)) * ((-1)^2 + 1^2))^0.5
  float res = Correlation<float>(x, y);
  EXPECT_FLOAT_EQ(1.0, res);
}

TEST(CorrelationTest, FloatReturnType) {
  vector<float> x = {1.0, 2.0, 4.0, 6.0};
  vector<float> y = {1.0, 5.0, 3.0, 1.0};
  float res = Correlation<float>(x, y);
  EXPECT_FLOAT_EQ(-0.274774, res);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
