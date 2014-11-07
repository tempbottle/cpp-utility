// Copyright 2014 ronaflx
#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "strings/stringprintf.h"
#include "util/csv/parser.h"
#include "util/csv/writer.h"

TEST(CSVTester, TestCSVWriter) {
  {
    std::ostringstream sout;
    std::vector<std::string> csv_record = { "\"ab\"", "cd\n", "ef" };
    csv::Writer<std::string> writer(csv_record, &sout);
    writer.WriteRecord();
    EXPECT_EQ(sout.str(), "\"\"\"ab\"\"\",\"cd\n\",ef\n");
  }
  {
    std::ostringstream sout;
    std::vector<std::string> csv_record = { "ab", "cd", "ef" };
    csv::Writer<std::string> writer(csv_record, &sout);
    writer.WriteRecord();
    EXPECT_EQ(sout.str(), "ab,cd,ef\n");
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

TEST(CSVTester, CSVParser) {
  {
    StringPiece string_piece(kMultTable);
    csv::Parser parser(string_piece);
    int num_of_lines = 0;
    for (const auto& record : parser) {
      std::vector<std::string> field = record.fields();
      EXPECT_EQ(field.size(), 11u);
      for (std::vector<std::string>::size_type i = 0; i < field.size(); i++) {
        EXPECT_EQ(field[i], StringPrintf("%u", i * num_of_lines));
      }
      num_of_lines++;
    }
    EXPECT_EQ(num_of_lines, 11);
  }
  {
    StringPiece string_piece(kMultTableTabs);
    csv::Parser parser(string_piece, '\t');
    int num_of_lines = 0;
    for (const auto& record : parser) {
      const std::vector<std::string>& field = record.fields();
      EXPECT_EQ(field.size(), 11u);
      for (std::vector<std::string>::size_type i = 0; i < field.size(); i++) {
        EXPECT_EQ(field[i], StringPrintf("%u", i * num_of_lines));
      }
      num_of_lines++;
    }
    EXPECT_EQ(num_of_lines, 11);
  }
}
TEST(CSVTester, EmptyLeadingLines) {
  {
    StringPiece string_piece(kCsvEmptyLeadingLines);
    csv::Parser parser(string_piece);
    std::vector<std::vector<std::string>> expected { { "" }, { "" }, {
        "George W. Bush", "January 20, 2001", "Republican" }, { "Barack Obama",
        "January 20, 2009", "Democratic" } };
    std::vector<std::vector<std::string>> actual;
    for (const auto record : parser) {
      actual.push_back(record.fields());
    }
    EXPECT_EQ(actual, expected);
  }
}

TEST(CSVTester, Newlines) {
  StringPiece string_piece(kNewlineFileContent);
  csv::Parser parser(string_piece);
  std::vector<std::vector<std::string>> expected { { "Google", "Rocks" }, {
      "Google", "Rocks" }, { "Google", "Rocks" }, { "Google", "Rocks" } };
  std::vector<std::vector<std::string>> actual;
  for (const auto record : parser) {
    actual.push_back(record.fields());
  }
  EXPECT_EQ(actual, expected);
}

TEST(CSVTester, MissingNewline) {
  StringPiece string_piece(kMissingNewlineContent);
  csv::Parser parser(string_piece);
  std::vector<std::vector<std::string>> expected { { "Google", "Rocks" }, {
      "Google", "Rocks" }, { "Google", "Rocks" }, { "Google", "Rocks" } };
  std::vector<std::vector<std::string>> actual;
  for (const auto record : parser) {
    actual.push_back(record.fields());
  }
  EXPECT_EQ(actual, expected);
}

TEST(CSVTester, QuotedNewline) {
  StringPiece string_piece(kQuotedNewlineContent);
  csv::Parser parser(string_piece);
  std::vector<std::vector<std::string>> expected {
    { "There", "is", "a", "newline", "here" },
    { "There,is\na", "newline", "here" },
    { "There,is\na", "newline", "here" },
    { "There,is\na", "newline", "here" }
  };

  std::vector<std::vector<std::string>> actual;
  for (const auto record : parser) {
    actual.push_back(record.fields());
  }
  EXPECT_EQ(actual, expected);
}

TEST(CSVTester, Misc) {
  StringPiece string_piece(kMisc);
  csv::Parser parser(string_piece);
  std::vector<std::vector<std::string>> expected { { "", "", "", "", "", "" }, {
      "" }, { "" }, {/* Error occur */}, { "asdfasdf" } };
  std::vector<std::vector<std::string>> actual;
  for (const auto record : parser) {
    actual.push_back(record.fields());
  }
  EXPECT_EQ(actual, expected);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
