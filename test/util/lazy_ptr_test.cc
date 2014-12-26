#include "util/lazy_ptr.h"

#include <gtest/gtest.h>
#include <string>
#include <atomic>
#include <thread>
#include <vector>

std::atomic_int call_constructor(0);

class TestClass {
 public:
  explicit TestClass(int a) : a_(a) { call_constructor++; }
  TestClass(int a, const std::string& b) : a_(a), b_(b) { call_constructor++; }
  TestClass(int a, const std::string& b, double c) : a_(a), b_(b), c_(c) {
    call_constructor++;
  }
  int a_;
  std::string b_;
  double c_;
  ~TestClass() {
    // This should never be called.
    EXPECT_TRUE(false);
  }
};

LazyPtr<TestClass, int> test_ptr_1 = {1};
LazyPtr<TestClass, int, std::string> test_ptr_2 = {1, "2"};
LazyPtr<TestClass, int, std::string, double> test_ptr_3 = {1, "2", 3.0};

TEST(LazyPtrTest, SimpleCase) {
  int curr_call_constructor = call_constructor;
  EXPECT_EQ(1, test_ptr_1->a_);
  EXPECT_EQ(curr_call_constructor + 1, call_constructor);
  EXPECT_EQ("2", test_ptr_2->b_);
  EXPECT_EQ(curr_call_constructor + 2, call_constructor);
  EXPECT_EQ(3.0, test_ptr_3->c_);
  EXPECT_EQ(curr_call_constructor + 3, call_constructor);
}

void ThreadFunction(const LazyPtr<TestClass, int>& test_ptr,
                    int expected_value) {
  EXPECT_EQ(expected_value, test_ptr->a_);
}

TEST(LazyPtrTest, MultiThreadTest) {
  const int num_thread = 1000;
  LazyPtr<TestClass, int> ptr = {1};
  std::vector<std::thread> thread_pool;
  int curr_call_constructor = call_constructor;
  for (int i = 0; i < num_thread; i++) {
    thread_pool.push_back(std::thread(ThreadFunction, std::ref(ptr), 1));
  }
  for (int i = 0; i < num_thread; i++) thread_pool[i].join();
  EXPECT_EQ(curr_call_constructor + 1, call_constructor);
}

int main (int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
