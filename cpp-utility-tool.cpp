//============================================================================
// Name        : cpp-utility-tool.cpp
// Author      : ronaflx
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "base/macros.h"
#include "util/registerer.h"
using namespace std;

class Builder {
 public:
  virtual void Build(const char* sentence) = 0;
  virtual ~Builder() {
  }
};
DEFINE_REGISTERER(Builder);
#define REGISTER_BUILDER(name) REGISTER_ENTITY(name, Builder)

class TermBuilder : public Builder {
 public:
  virtual void Build(const char* sentence) {
    cout << "TermBuilder" << endl;
  }
};
REGISTER_BUILDER(TermBuilder);

class WordBuilder : public Builder {
 public:
  virtual void Build(const char* sentence) {
    cout << "WordBuilder" << endl;
  }
};
REGISTER_BUILDER(WordBuilder);

void TestRegister() {
  Builder* term_builder = BuilderRegisterer::CreateByName("TermBuilder");
  Builder* word_builder = BuilderRegisterer::CreateByName("WordBuilder");
  term_builder->Build("some builder");
  word_builder->Build("some builder");
}

int main() {
  TestRegister();
  return 0;
}
