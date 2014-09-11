
// Copyright 2014 ronaflx
#include <gtest/gtest.h>

#include "util/copy_on_write.h"

TEST(CopyOnWriteTest, SingleInstance) {
  CopyOnWrite<int> n;
  EXPECT_EQ(n.get_ref(), 1);
}

TEST(CopyOnWriteTest, Copy) {
  CopyOnWrite<int> n;
  CopyOnWrite<int> m = n;
  CopyOnWrite<int> x = n;
  EXPECT_EQ(3, n.get_ref());
  EXPECT_EQ(3, m.get_ref());
  EXPECT_EQ(3, x.get_ref());
}

TEST(CopyOnWrite, Relase) {
  CopyOnWrite<int> m;
  EXPECT_EQ(1, m.get_ref());
  {
    CopyOnWrite<int> n;
    EXPECT_EQ(1, n.get_ref());
    n = m;
    EXPECT_EQ(2, n.get_ref());
    EXPECT_EQ(2, n.get_ref());
  }
  EXPECT_EQ(1, m.get_ref());
  CopyOnWrite<int> n = m;
  EXPECT_EQ(2, n.get_ref());
  EXPECT_EQ(2, m.get_ref());
  n.get_mutable();
  EXPECT_NE(n.get_mutable(), m.get_mutable());
  EXPECT_EQ(n.get(), m.get());
}

TEST(CopyOnWriteTest, Assign) {
  CopyOnWrite<int> n;
  CopyOnWrite<int> m;
  CopyOnWrite<int> x;
  n = m = x;
  EXPECT_EQ(3, n.get_ref());
  EXPECT_EQ(3, m.get_ref());
  EXPECT_EQ(3, x.get_ref());
  // Reassign, the result should stay same
  n = m = x;
  EXPECT_EQ(3, n.get_ref());
  EXPECT_EQ(3, m.get_ref());
  EXPECT_EQ(3, x.get_ref());
  CopyOnWrite<int> y;
  CopyOnWrite<int> z;
  (x = z) = y;
  EXPECT_EQ(2, n.get_ref());
  EXPECT_EQ(2, m.get_ref());

  EXPECT_EQ(2, x.get_ref());
  EXPECT_EQ(2, y.get_ref());
  EXPECT_EQ(1, z.get_ref());

  EXPECT_EQ(&n.get(), &m.get());
  EXPECT_EQ(&x.get(), &y.get());
  EXPECT_NE(&x.get(), &z.get());
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
