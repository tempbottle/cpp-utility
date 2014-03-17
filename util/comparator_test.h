#ifndef UTIL_COMPARATOR_TEST_H_
#define UTIL_COMPARATOR_TEST_H_
#include <vector>
#include <gtest/gtest.h>

#include "util/comparator.h"

TEST(ComparatorUtilTest, FieldFunctorTest) {
  class Internal {
   public:
    int int_field;
  } internal;
  internal.int_field = 10;
  Field<Internal, int> field_getter(&Internal::int_field);
  EXPECT_EQ(field_getter(internal), 10);
  internal.int_field = 20;
  EXPECT_EQ(field_getter(internal), 20);
}

TEST(ComparatorUtilTest, GetterFunctorTest) {
  class Internal {
   public:
    int get_int_field() const {
      return int_field;
    }
    void set_int_field(int value) {
      int_field = value;
    }
    const std::vector<int>& get_class_field() const {
      return class_field;
    }
   private:
    int int_field;
    std::vector<int> class_field;
  };
  Internal internal;
  Getter<Internal, int> pod_getter(&Internal::get_int_field);
  internal.set_int_field(10);
  EXPECT_EQ(pod_getter(internal), 10);
  internal.set_int_field(20);
  EXPECT_EQ(pod_getter(internal), 20);

  // Use reference as template param here.
  Getter<Internal, const std::vector<int>&> class_getter(
      &Internal::get_class_field);
}

#endif /* UTIL_COMPARATOR_TEST_H_ */
