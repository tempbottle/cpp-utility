// Copyright 2014 ronaflx
#include "util/json/json.h"

#include <string>
#include <gmock/gmock.h>

using json::Json;

TEST(JsonTest, SimpleJsonValueGood) {
  Json json;
  const std::string integer = "1234567";
  ASSERT_TRUE(json.Parse(integer));
  EXPECT_EQ(Json::JINT, json.Type());
  EXPECT_EQ("1234567", json.Dump());

  const std::string true_value = "true\n";
  ASSERT_TRUE(json.Parse(true_value));
  EXPECT_EQ(Json::JBOOL, json.Type());
  EXPECT_EQ("true", json.Dump());

  const std::string false_value = "false ";
  ASSERT_TRUE(json.Parse(false_value));
  EXPECT_EQ(Json::JBOOL, json.Type());
  EXPECT_EQ("false", json.Dump());

  const std::string null_value = "null";
  ASSERT_TRUE(json.Parse(null_value));
  EXPECT_EQ(Json::JNUL, json.Type());
  EXPECT_EQ("null", json.Dump());
}

TEST(JsonTest, SimpleJsonValueBad) {
  Json json;
  const std::string invalid_true = "teru";
  ASSERT_FALSE(json.Parse(invalid_true));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
  const std::string invalid_false = "fasle";
  ASSERT_FALSE(json.Parse(invalid_false));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
  const std::string invalid_null = "nlul";
  ASSERT_FALSE(json.Parse(invalid_null));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
  const std::string invalid_suffix = "null abcd";
  ASSERT_FALSE(json.Parse(invalid_null));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
  const std::string null_empty = "";
  ASSERT_FALSE(json.Parse(null_empty));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
}

TEST(JsonTest, OnlyPrefixMatchForSimpleValue) {
  Json json;
  const std::string invalid_true = "truea";
  EXPECT_FALSE(json.Parse(invalid_true));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
  const std::string invalid_false = "false\f";
  EXPECT_FALSE(json.Parse(invalid_false));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
  const std::string invalid_null = "null\b";
  EXPECT_FALSE(json.Parse(invalid_null));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
}

TEST(JsonTest, ParseJsonStringGood) {
  Json json;
  const std::string control_character = "\"\\u0010\\u0003\\u0001\\u0002\"";
  ASSERT_TRUE(json.Parse(control_character));
  EXPECT_EQ(Json::JSTRING, json.Type());
  EXPECT_EQ("\x10\x3\x1\x2", json.StringValue());
  EXPECT_EQ("\"\\u0010\\u0003\\u0001\\u0002\"", json.Dump());

  const std::string escaped_character = "\"\\\\\\\"\\b\\f\\n\\r\\t\"";
  ASSERT_TRUE(json.Parse(escaped_character));
  EXPECT_EQ(Json::JSTRING, json.Type());
  EXPECT_EQ("\\\"\b\f\n\r\t", json.StringValue());
  EXPECT_EQ("\"\\\\\\\"\\b\\f\\n\\r\\t\"", json.Dump());

  const std::string& normal_string = "\"normal_string\"";
  ASSERT_TRUE(json.Parse(normal_string));
  EXPECT_EQ(Json::JSTRING, json.Type());
  EXPECT_EQ("normal_string", json.StringValue());
  EXPECT_EQ("\"normal_string\"", json.Dump());

  const std::string utf8_string = "\"\\u0080\\u0800\"";
  ASSERT_TRUE(json.Parse(utf8_string));
  EXPECT_EQ(Json::JSTRING, json.Type());
  EXPECT_EQ("\xC2\x80\xE0\xA0\x80", json.StringValue());
  // TODO(ronaflx): enable this test case.
  // EXPECT_EQ("\"\\u0080\\u0800\"", json.Dump());
}

TEST(JsonTest, ParseJsonStringBad) {
  Json json;
  const std::string bad_control = "\"\x19\"";
  ASSERT_FALSE(json.Parse(bad_control));
  EXPECT_EQ(Json::UNESCAPED_CTRL, json.ErrorType());

  const std::string bad_end = "\"abcd\\\"";
  ASSERT_FALSE(json.Parse(bad_end));
  EXPECT_EQ(Json::END_ERROR, json.ErrorType());

  std::string invalid_escaped = "\"abcd\\uabq1\"";
  ASSERT_FALSE(json.Parse(invalid_escaped));
  EXPECT_EQ(Json::INVALID_ESCAPED, json.ErrorType());

  // Unocide code point must be 4 digits.
  invalid_escaped = "\"abcd\\uab1\"";
  ASSERT_FALSE(json.Parse(invalid_escaped));
  EXPECT_EQ(Json::INVALID_ESCAPED, json.ErrorType());

  const std::string bad_escaped = "\"\\x\"";
  ASSERT_FALSE(json.Parse(bad_escaped));
  EXPECT_EQ(Json::BAD_ESCAPED, json.ErrorType());
}

TEST(JsonTest, ParseJsonNumberGood) {
  Json json;
  // Negative Integer
  ASSERT_TRUE(json.Parse("-10000"));
  EXPECT_EQ(Json::JINT, json.Type());
  EXPECT_EQ(-10000, json.IntValue());
  EXPECT_EQ("-10000", json.Dump());

  // Zero
  ASSERT_TRUE(json.Parse("0"));
  EXPECT_EQ(Json::JINT, json.Type());
  EXPECT_EQ(0, json.IntValue());
  EXPECT_EQ("0", json.Dump());

  // Positive / negative float number.
  ASSERT_TRUE(json.Parse("1231.123201"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(1231.123201, json.RealValue());
  EXPECT_EQ("1231.123201", json.Dump());

  ASSERT_TRUE(json.Parse("-5678.01230"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(-5678.0123, json.RealValue());
  EXPECT_EQ("-5678.0123", json.Dump());

  // Positive / negative scientific notation.
  ASSERT_TRUE(json.Parse("2e14"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(2e14, json.RealValue());
  EXPECT_EQ("2e+14", json.Dump());

  ASSERT_TRUE(json.Parse("-12e14"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(-12e14, json.RealValue());
  EXPECT_EQ("-1.2e+15", json.Dump());

  // Negative exponent.
  ASSERT_TRUE(json.Parse("3e-4"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(3e-4, json.RealValue());
  EXPECT_EQ("0.0003", json.Dump());

  ASSERT_TRUE(json.Parse("-2e-5"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(-2e-5, json.RealValue());
  EXPECT_EQ("-2e-05", json.Dump());

  // Decimal and scientific notation.
  ASSERT_TRUE(json.Parse("32.12e5"));
  EXPECT_EQ(Json::JREAL, json.Type());
  ASSERT_TRUE(json.Parse("32.12e+5"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(32.12e5, json.RealValue());
  ASSERT_TRUE(json.Parse("-30.2e5"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(-30.2e5, json.RealValue());

  // Decimal and scientific notation. Negative exponent.
  ASSERT_TRUE(json.Parse("32.12e-5"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(32.12e-5, json.RealValue());
  ASSERT_TRUE(json.Parse("-30.2e-5"));
  EXPECT_EQ(Json::JREAL, json.Type());
  EXPECT_DOUBLE_EQ(-30.2e-5, json.RealValue());
}

TEST(JsonTest, ParseJsonNumberBad) {
  Json json;
  // Only sign error
  ASSERT_FALSE(json.Parse("-"));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());

  // Leading zero error.
  ASSERT_FALSE(json.Parse("00"));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());
  ASSERT_FALSE(json.Parse("-01"));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());

  // No digit for dicimal part.
  ASSERT_FALSE(json.Parse("123."));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());

  // Not digit for scientific notation part.
  ASSERT_FALSE(json.Parse("123.0e"));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());
  ASSERT_FALSE(json.Parse("123.0e+"));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());
  ASSERT_FALSE(json.Parse("123.0e-"));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());
  // Invalid suffix.
  ASSERT_FALSE(json.Parse("123.0ae"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
  ASSERT_FALSE(json.Parse("123.0ea"));
  EXPECT_EQ(Json::INVALID_NUM, json.ErrorType());
  ASSERT_FALSE(json.Parse("123.0e10a"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
}

TEST(JsonTest, ParseJsonArrayGood) {
  Json json;
  ASSERT_TRUE(json.Parse("[]"));
  EXPECT_EQ(Json::JARRAY, json.Type());
  {
    const Json::Array& json_array = json.ArrayValue();
    EXPECT_EQ(0, json_array.size());
  }
  EXPECT_EQ("[\n]", json.Dump());

  ASSERT_TRUE(json.Parse("[true, false]"));
  {
    const Json::Array& json_array = json.ArrayValue();
    EXPECT_EQ(2, json_array.size());
    EXPECT_EQ(Json::JBOOL, json_array[0].Type());
    EXPECT_EQ(true, json_array[0].BoolValue());
    EXPECT_EQ(Json::JBOOL, json_array[1].Type());
    EXPECT_EQ(false, json_array[1].BoolValue());
  }

  ASSERT_TRUE(json.Parse("[true, 1e20, \"abcde\\\\\\\"\"]"));
  EXPECT_EQ("[\n  true,\n  1e+20,\n  \"abcde\\\\\\\"\"\n]", json.Dump(2));
  {
    const Json::Array& json_array = json.ArrayValue();
    EXPECT_EQ(3, json_array.size());
    EXPECT_EQ(Json::JBOOL, json_array[0].Type());
    EXPECT_EQ(true, json_array[0].BoolValue());
    EXPECT_EQ(Json::JREAL, json_array[1].Type());
    EXPECT_DOUBLE_EQ(1e20, json_array[1].RealValue());
    EXPECT_EQ(Json::JSTRING, json_array[2].Type());
    EXPECT_EQ("abcde\\\"", json_array[2].StringValue());
  }

  ASSERT_TRUE(json.Parse("[[], [1, 2, 3.0], [\"a\", \"b\", [\"c\"]]]"));
  EXPECT_EQ(
      "[\n  [\n  ],\n  [\n    1,\n    2,\n    3\n  ],\n  [\n    \"a\",\n    "
      "\"b\",\n    [\n      \"c\"\n    ]\n  ]\n]",
      json.Dump(2));

  {
    const Json::Array& json_array = json.ArrayValue();
    EXPECT_EQ(3, json_array.size());
    EXPECT_EQ(Json::JARRAY, json_array[0].Type());
    EXPECT_EQ(Json::JARRAY, json_array[1].Type());
    EXPECT_EQ(Json::JARRAY, json_array[2].Type());

    // sub array 0
    EXPECT_EQ(0, json_array[0].ArrayValue().size());
    // sub array 1
    EXPECT_EQ(3, json_array[1].ArrayValue().size());
    EXPECT_EQ(Json::JINT, json_array[1][0].Type());
    EXPECT_EQ(1, json_array[1][0].IntValue());
    EXPECT_EQ(Json::JINT, json_array[1][1].Type());
    EXPECT_EQ(2, json_array[1][1].IntValue());
    EXPECT_EQ(Json::JREAL, json_array[1][2].Type());
    EXPECT_DOUBLE_EQ(3.0, json_array[1][2].RealValue());
    // sub array 2
    EXPECT_EQ(3, json_array[2].ArrayValue().size());
    EXPECT_EQ(Json::JSTRING, json_array[2][0].Type());
    EXPECT_EQ("a", json_array[2][0].StringValue());
    EXPECT_EQ(Json::JSTRING, json_array[2][1].Type());
    EXPECT_EQ("b", json_array[2][1].StringValue());
    EXPECT_EQ(Json::JARRAY, json_array[2][2].Type());
    EXPECT_EQ(1, json_array[2][2].ArrayValue().size());
    EXPECT_EQ("c", json_array[2][2][0].StringValue());
  }
}

TEST(JsonTest, ParseJsonArrayBad) {
  Json json;
  ASSERT_FALSE(json.Parse("["));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());

  ASSERT_FALSE(json.Parse("[1123 12321]"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());

  ASSERT_FALSE(json.Parse("[1123, 12321,]"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
}

TEST(JsonTest, ParseJsonObjectGood) {
  Json json;
  ASSERT_TRUE(json.Parse("{}"));
  EXPECT_EQ("{\n}", json.Dump());
  EXPECT_EQ(Json::JOBJECT, json.Type());
  EXPECT_EQ(0, json.ObjectValue().size());

  ASSERT_TRUE(json.Parse("{\"abcd\" : 123, \"efg\": true, \"hig\": 1.2}"));
  EXPECT_EQ(
      "{\n    \"abcd\":\n    123,\n    \"efg\":\n    true,\n    \"hig\":\n    "
      "1.2\n}",
      json.Dump());
  EXPECT_EQ(Json::JOBJECT, json.Type());
  EXPECT_EQ(3, json.ObjectValue().size());
  EXPECT_EQ(Json::JINT, json["abcd"].Type());
  EXPECT_EQ(123, json["abcd"].IntValue());
  EXPECT_EQ(Json::JBOOL, json["efg"].Type());
  EXPECT_EQ(true, json["efg"].BoolValue());
  EXPECT_EQ(Json::JREAL, json["hig"].Type());
  EXPECT_EQ(1.2, json["hig"].RealValue());

  ASSERT_TRUE(
      json.Parse("{\"123\": 123, \"456\": {\"123\": 456}, \"2\": \"22\"}"));
  EXPECT_EQ(
      "{\n  \"123\":\n  123,\n  \"2\":\n  \"22\",\n  \"456\":\n  {\n    "
      "\"123\":\n    456\n  }\n}",
      json.Dump(2));
  EXPECT_EQ("{\"123\": 123, \"2\": \"22\", \"456\": {\"123\": 456}}",
            json.ToString());
  EXPECT_EQ(Json::JOBJECT, json.Type());
  EXPECT_EQ(3, json.ObjectValue().size());
  EXPECT_EQ(Json::JINT, json["123"].Type());
  EXPECT_EQ(123, json["123"].IntValue());
  EXPECT_EQ(Json::JOBJECT, json["456"].Type());
  EXPECT_EQ(Json::JINT, json["456"]["123"].Type());
  EXPECT_EQ(456, json["456"]["123"].IntValue());
  EXPECT_EQ(Json::JSTRING, json["2"].Type());
  EXPECT_EQ("22", json["2"].StringValue());
}

TEST(JsonTest, ParseJsonObjectBad) {
  Json json;
  ASSERT_FALSE(json.Parse("{"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());

  ASSERT_FALSE(json.Parse("{\"1123\" 12321}"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());

  ASSERT_FALSE(json.Parse("{\"1123\": 12321,}"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());

  ASSERT_FALSE(json.Parse("{\"1123\": 12321 \"456\"}"));
  EXPECT_EQ(Json::UNEXPECTED_CHAR, json.ErrorType());
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
