#include <iostream>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <iterator>

#include "base/macros.h"
#include "base/sysinfo.h"
#include "base/timer.h"
#include "strings/string_piece.h"
#include "util/csv/parser.h"
#include "util/csv/writer.h"
#include "util/singleton.h"
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

void TestSingleton() {
  int* ptr0 = Singleton<int>::Get();
  int* ptr1 = Singleton<int>::Get();
  assert(ptr0 == ptr1);
  if (ptr0 == ptr1) {
    cout << "equal address" << endl;
  }
}

void TestStringPiece() {
  static const char* ktest_string = "StringPiece Test String";
  {
    StringPiece string_piece;
    assert(string_piece.data() == nullptr);
    assert(string_piece.length() == 0u);
  }
  {
    StringPiece string_piece(ktest_string);
    assert(string_piece.length() == strlen(ktest_string));
    printf("%s\n", string_piece.ToString().c_str());
    string_piece.remove_prefix(5);
    assert(string_piece.length() == strlen(ktest_string) - 5);
    printf("%s\n", string_piece.ToString().data());
    string_piece.remove_suffix(5);
    assert(string_piece.length() == strlen(ktest_string) - 10);
    printf("%s\n", string_piece.ToString().data());
  }

  // Test operator ==
  {
    StringPiece x(ktest_string), y(ktest_string);
    assert(x == y);
  }
  // Test operator !=, <, >
  {
    StringPiece x("abc"), y("xyz");
    assert(x != y);
    assert(x < y);
    assert(y > x);
  }
  // Test operator <=, >=
  {
    StringPiece x("abc"), y("abc"), z("abd");
    assert(x >= y);
    assert(y <= x);
    assert(x <= z);
    assert(z >= x);
  }
}

const char kMultTable[] = "0,0,0,0,0,0,0,0,0,0,0\n"
    "0,1,2,3,4,5,6,7,8,9,10\n"
    "0,2,4,6,8,10,12,14,16,18,20\n"
    "0,3,6,9,12,15,18,21,24,27,30\n"
    "0,4,8,12,16,20,24,28,32,36,40\n"
    "0,5,10,15,20,25,30,35,40,45,50\n"
    "0,6,12,18,24,30,36,42,48,54,60\n"
    "0,7,14,21,28,35,42,49,56,63,70\n"
    "0,8,16,24,32,40,48,56,64,72,80\n"
    "0,9,18,27,36,45,54,63,72,81,90\n"
    "0,10,20,30,40,50,60,70,80,90,100\n";

const char kMultTableTabs[] = "0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n"
    "0\t1\t2\t3\t4\t5\t6\t7\t8\t9\t10\n"
    "0\t2\t4\t6\t8\t10\t12\t14\t16\t18\t20\n"
    "0\t3\t6\t9\t12\t15\t18\t21\t24\t27\t30\n"
    "0\t4\t8\t12\t16\t20\t24\t28\t32\t36\t40\n"
    "0\t5\t10\t15\t20\t25\t30\t35\t40\t45\t50\n"
    "0\t6\t12\t18\t24\t30\t36\t42\t48\t54\t60\n"
    "0\t7\t14\t21\t28\t35\t42\t49\t56\t63\t70\n"
    "0\t8\t16\t24\t32\t40\t48\t56\t64\t72\t80\n"
    "0\t9\t18\t27\t36\t45\t54\t63\t72\t81\t90\n"
    "0\t10\t20\t30\t40\t50\t60\t70\t80\t90\t100\n";

const char kCsvEmptyLeadingLines[] = "\n\n"
    "George W. Bush,\"January 20, 2001\",Republican\n"
    "Barack Obama,\"January 20, 2009\",Democratic\n";

const char kNewlineFileContent[] = "Google,Rocks\r"
    "Google,Rocks\r\n"
    "Google,Rocks\r"
    "Google,Rocks\n";

const char kMissingNewlineContent[] = "Google,Rocks\n"
    "Google,Rocks\n"
    "Google,Rocks\n"
    "Google,Rocks";

const char kQuotedNewlineContent[] = "There,is,a,newline,here\n"
    "\"There,is\na\",newline,here\n"
    "\"There,is\r\na\",newline,here\n"
    "\"There,is\ra\",newline,here\n";

const char kMisc[] = ",,,,,\n"
    "\n"
    "\n"
    "asdfsd\"asdfasdf";
void TestCSVParserDetail(const StringPiece& string_piece, char delim = ',') {
  printf("%s\n", string_piece.data());
  csv::Parser parser(string_piece, delim);
  for (const auto& record : parser) {
    vector<string> field = record.fields();
    printf("Lines: %lld", record.number());
    for (const auto& i : field) {
      printf("[comma]%s", i.c_str());
    }
    printf("\n");
  }
}

void TestCSVParser() {
  {
    StringPiece string_piece(kMultTable);
    TestCSVParserDetail(string_piece);
  }
  {
    StringPiece string_piece(kMultTableTabs);
    TestCSVParserDetail(string_piece, '\t');
  }
  {
    StringPiece string_piece(kCsvEmptyLeadingLines);
    TestCSVParserDetail(string_piece);
  }
  {
    StringPiece string_piece(kNewlineFileContent);
    TestCSVParserDetail(string_piece);
  }
  {
    StringPiece string_piece(kMissingNewlineContent);
    TestCSVParserDetail(string_piece);
  }
  {
    StringPiece string_piece(kQuotedNewlineContent);
    TestCSVParserDetail(string_piece);
  }
  {
    StringPiece string_piece(kMisc);
    TestCSVParserDetail(string_piece);
  }
}

void TestCSVWriter() {
  {
    std::vector<std::string> csv_record = { "\"ab\"", "cd\n", "ef" };
    csv::Writer<std::string> writer(csv_record, &cout);
    writer.WriteRecord();
  }
  {
    std::vector<std::string> csv_record = { "ab", "cd", "ef" };
    csv::Writer<std::string> writer(csv_record, &cout);
    writer.WriteRecord();
  }
}

int main() {
  TestRegister();
  TestSingleton();
  TestStringPiece();
  TestCSVParser();
  TestCSVWriter();
  printf("%d\n", NumCPUs());
  printf("%lf\n", NominalCPUFrequency());
  printf("%lf\n", CycleClockFrequency());
  WallTimer wall_timer;
  wall_timer.Start();
  char text_buf[1024];
  while (gets(text_buf)) {
    printf("%s %lf %lld\n", text_buf, wall_timer.Get(), wall_timer.GetInMs());
  }
  return 0;
}
