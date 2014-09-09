// Copyright 2014 ronaflx
#include "util/comparator.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <utility>
#include <functional>
using std::pair;
using std::vector;

TEST(ComparatorUtilTest, FieldFunctorTest) {
  typedef pair<int, int> pairII;
  pairII p(10, 0);
  Field<pairII, int> field_getter(&pairII::first);
  EXPECT_EQ(field_getter(p), 10);
  p.first = 20;
  EXPECT_EQ(field_getter(p), 20);
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

template<class P>
void Verify(const vector<P>& vec, Field<P, int> field, Field<P, int> iter_field,
            int N, bool order_i = true, bool order_j = true) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      EXPECT_EQ(
          field(vec[i * N + j]),
          iter_field(
              std::make_pair(order_i ? i : N - i - 1,
                             order_j ? j : N - j - 1)));
    }
  }
}
TEST(ComparatorUitlTest, ChainComparatorTest) {
  typedef pair<int, int> PairII;
  typedef Field<PairII, int> PairField;
  std::vector<PairII> pairs;
  const int N = 3;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      pairs.push_back(std::make_pair(i, j));
    }
  }
  EXPECT_EQ(pairs.size(), N * N);
  // original sort
  std::sort(pairs.begin(), pairs.end());
  Verify(pairs, PairField(&PairII::first), PairField(&PairII::first), N);
  Verify(pairs, PairField(&PairII::second), PairField(&PairII::second), N);

  // sort by first increasingly
  std::sort(pairs.begin(), pairs.end(), MakeOrderBy(PairField(&PairII::first)));
  Verify(pairs, PairField(&PairII::first), PairField(&PairII::first), N);

  // sort by first decreasingly
  std::sort(pairs.begin(), pairs.end(),
            MakeOrderBy(PairField(&PairII::first), std::greater<int>()));
  Verify(pairs, PairField(&PairII::first), PairField(&PairII::first), N, false,
         true);

  // sort by second increasingly
  std::sort(pairs.begin(), pairs.end(),
            MakeOrderBy(PairField(&PairII::second)));
  Verify(pairs, PairField(&PairII::second), PairField(&PairII::first), N);

  // sort by second decreasingly
  std::sort(pairs.begin(), pairs.end(),
            MakeOrderBy(PairField(&PairII::second), std::greater<int>()));
  Verify(pairs, PairField(&PairII::second), PairField(&PairII::first), N, false,
         true);

  // sort by Chain Comparators
  std::sort(
      pairs.begin(),
      pairs.end(),
      MakeChainComparators(MakeOrderBy(PairField(&PairII::first)),
                           MakeOrderBy(PairField(&PairII::second))));
  Verify(pairs, PairField(&PairII::first), PairField(&PairII::first), N);
  Verify(pairs, PairField(&PairII::second), PairField(&PairII::second), N);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
