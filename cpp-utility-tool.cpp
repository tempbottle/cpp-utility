//============================================================================
// Name        : cpp-utility-tool.cpp
// Author      : ronaflx
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cassert>
#include "base/macros.h"
#include "base/sysinfo.h"
#include "base/timer.h"
#include "util/registerer.h"
#include "util/singleton.h"

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

void TestSingleton() {
  int* ptr0 = Singleton<int>::Get();
  int* ptr1 = Singleton<int>::Get();
  assert(ptr0 == ptr1);
  if (ptr0 == ptr1) {
    cout << "equal address" << endl;
  }
}

int main() {
  TestRegister();
  TestSingleton();
  printf("%d\n", NumCPUs());
  printf("%lf\n", NominalCPUFrequency());
  printf("%lf\n", CycleClockFrequency());
  WallTimer wall_timer;
  wall_timer.Start();
  char text_buf[1024];
  while(gets(text_buf)) {
    printf("%s %lf %lld\n", text_buf, wall_timer.Get(), wall_timer.GetInMs());
  }
  return 0;
}