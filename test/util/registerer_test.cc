// Copyright 2014 ronaflx
#include "util/registerer.h"
#include <gtest/gtest.h>
#include <string>
#include <memory>

class Builder {
 public:
  virtual std::string Build(const char* sentence) = 0;
  virtual ~Builder() {
  }
};
DEFINE_REGISTERER(Builder);
#define REGISTER_BUILDER(name) REGISTER_ENTITY(name, Builder)

class TermBuilder : public Builder {
 public:
  virtual string Build(const char* sentence) {
    return "TermBuilder";
  }
};
REGISTER_BUILDER(TermBuilder);

class WordBuilder : public Builder {
 public:
  virtual string Build(const char* sentence) {
    return "WordBuilder";
  }
};
REGISTER_BUILDER(WordBuilder);

TEST(TestRegister, VirtualFunctionTest) {
  std::unique_ptr<Builder> term_builder(
      BuilderRegisterer::CreateByName("TermBuilder"));
  std::unique_ptr<Builder> word_builder(
      BuilderRegisterer::CreateByName("WordBuilder"));
  EXPECT_EQ(term_builder->Build("some builder"), "TermBuilder");
  EXPECT_EQ(word_builder->Build("some builder"), "WordBuilder");
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
