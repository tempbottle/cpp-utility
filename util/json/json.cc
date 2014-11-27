// CopyRight 2014 ronaflx
#include "util/json/json.h"

#include <cmath>
#include <cstdlib>

#include "base/integral_types.h"
#include "base/macros.h"
#include "strings/encoding.h"

namespace json {

// Helper function and class.
static const Json default_json;

// Helper
const char kEscCharEnd[] = {'\"', '\\', 'b', 'f', 'n', 'r', 't', '/'};
const char kEscChar[] = {'\"', '\\', '\b', '\f', '\n', '\r', '\t', '/'};
const char* KEscString[] = {"\\\"", "\\\\", "\\b", "\\f",
                            "\\n",  "\\r",  "\\t", "\\/"};
const int kEscSize = arraysize(kEscCharEnd);
static_assert(arraysize(kEscChar) == kEscSize, "should be equal");
static_assert(arraysize(KEscString) == kEscSize, "should be equal");

// Json need to deal with 0xe280a8 and 0xe280a9 carefully.
// This two character may break json feeds.
// The order of this function:
// 1. Process "escape character". Some escape character are control character.
// 2. Process "control character". "control character" are below 32
// 3. Process 0xe290a8(LINE SEPARATOR) 0xe280a9(PARAGRAPH SEPARATOR)
static void DumpString(const std::string& value, std::string* out) {
  out->append(1, '\"');
  size_t value_size = value.size();
  for (size_t i = 0; i < value_size; i++) {
    char ch = value[i];
    size_t escape_id = std::find(kEscChar, kEscChar + kEscSize, ch) - kEscChar;
    if (escape_id != kEscSize) {
      out->append(KEscString[i]);
    } else if (ch < 0x20) {
      char buf[8];
      snprintf(buf, sizeof(buf), "\\u%04x", ch);
      out->append(buf);
    } else if (i + 2 < value_size && static_cast<unsigned char>(ch) == 0xe2 &&
               static_cast<unsigned char>(value[i + 1]) == 0x80 &&
               static_cast<unsigned char>(value[i + 2]) == 0xa8) {
      out->append("\\u2028");
    } else if (i + 2 < value_size && static_cast<unsigned char>(ch) == 0xe2 &&
               static_cast<unsigned char>(value[i + 1]) == 0x80 &&
               static_cast<unsigned char>(value[i + 2]) == 0xa9) {
      out->append("\\u2029");
    } else {
      out->append(1, ch);
    }
  }
}

// Json Parser. This class should not be invisible for external.
class JsonParser {
 public:
  JsonParser(const char* json_data, size_t length, Json* json)
      : json_data_(json_data, length), length_(length), json_(json) {}
  bool Parse();

 private:
  Json ParseInternal();
  StringPiece json_data_;
  size_t length_;
  Json* json_;

  // Function to help parse.
  void ConsumeWhiteSpace();
  char NextCharacter();
  bool IsHex(const std::string& number) const;
  bool IsNullOrSeparator(size_t pos) const;
  // Fail stats will only set to the root of json.
  void Fail(Json::error_type error, const std::string& expect,
            const std::string& actual) const;
  // Parse number and string into json object.
  bool ParseString(Json* json);
  bool ParseNumber(Json* json);
};

// Implementation Json class
const Json::Object Json::default_object_;
const Json::Array Json::default_array_;
const std::string Json::default_string_;

Json::Json() : json_value_(nullptr), valid_(true) {}

std::string Json::ToString() const {
  std::string result;
  json_value_->Dump(&result);
  return result;
}

const JsonValue* Json::GetJsonValue() const { return json_value_.get(); }

bool Json::IsValid() const { return valid_; }

void Json::clear() {
  json_value_.reset();
  valid_ = true;
  error_msg_.json_error_type = NO_ERROR;
}

bool Json::ParseFromString(const char* json_data, size_t length, Json* json) {
  JsonParser parser(json_data, length, json);
  return parser.Parse();
}

bool Json::ParseFromString(const char* json_data, Json* json) {
  return ParseFromString(json_data, strlen(json_data), json);
}

bool Json::ParseFromString(const std::string& json_data, Json* json) {
  return ParseFromString(json_data.c_str(), static_cast<int>(json_data.size()),
                         json);
}

bool Json::Parse(const char* json_data, size_t length) {
  return ParseFromString(json_data, length, this);
}

bool Json::Parse(const char* json_data) {
  return ParseFromString(json_data, this);
}

bool Json::Parse(const std::string& json_data) {
  return ParseFromString(json_data, this);
}

#define EXPORT_JSON_API(ret_type, name) \
  ret_type Json::name() const { return json_value_->name(); }

EXPORT_JSON_API(Json::type, Type)
EXPORT_JSON_API(double, RealValue)
EXPORT_JSON_API(int, IntValue)
EXPORT_JSON_API(bool, BoolValue)
EXPORT_JSON_API(const std::string&, StringValue)
EXPORT_JSON_API(const Json::Array&, ArrayValue)
#undef EXPORT_JSON_API

Json::error_type Json::ErrorType() const { return error_msg_.json_error_type; }
const Json::ErrorMessage& Json::ReportErrorMessage() const {
  return error_msg_;
}

// Implementation JsonValue class.
// These function is base class function.
// Some of them will be override by its subclass.
double JsonValue::RealValue() const { return 0; }

int JsonValue::IntValue() const { return 0; }

bool JsonValue::BoolValue() const { return false; }

const std::string& JsonValue::StringValue() const {
  return Json::default_string_;
}

const Json::Array& JsonValue::ArrayValue() const {
  return Json::default_array_;
}

const Json::Object& JsonValue::ObjectValue() const {
  return Json::default_object_;
}

const Json& JsonValue::operator[](size_t pos) const { return default_json; }

const Json& JsonValue::operator[](const std::string& key) const {
  return default_json;
}

// Implementation for each subclass for JsonValue.
// Function type(): return json type for each subclass.
// Function *Value(): Each subclass overrides its type of function.
// Function dump(): dump formatted text for each subclass.
class JsonNull : public JsonValue {
 public:
  virtual Json::type Type() const;
  virtual void Dump(std::string* out) const override;
};

Json::type JsonNull::Type() const { return Json::JNUL; }

void JsonNull::Dump(std::string* out) const { out->append("null"); }

class JsonDouble : public JsonValue {
 public:
  JsonDouble() : value_(0) {}
  explicit JsonDouble(double value) : value_(value) {}
  virtual double RealValue() const override;
  virtual Json::type Type() const;
  virtual void Dump(std::string*) const override;

 private:
  double value_;
};

double JsonDouble::RealValue() const { return value_; }

Json::type JsonDouble::Type() const { return Json::JREAL; }

void JsonDouble::Dump(std::string* out) const {
  static char buffer[32];
  snprintf(buffer, sizeof(buffer), "%.10g", value_);
  out->append(buffer);
}

class JsonInt : public JsonValue {
 public:
  JsonInt() : value_(0) {}
  explicit JsonInt(int value) : value_(value) {}
  virtual int IntValue() const override;
  virtual Json::type Type() const;
  virtual void Dump(std::string* out) const;

 private:
  int value_;
};

Json::type JsonInt::Type() const { return Json::JINT; }

int JsonInt::IntValue() const { return value_; }

void JsonInt::Dump(std::string* out) const {
  static char buffer[32];
  snprintf(buffer, sizeof(buffer), "%d", value_);
  out->append(buffer);
}

class JsonBool : public JsonValue {
 public:
  JsonBool() : value_(false) {}
  explicit JsonBool(bool value) : value_(value) {}
  virtual bool BoolValue() const override;
  virtual Json::type Type() const;
  virtual void Dump(std::string* out) const;

 private:
  bool value_;
};

Json::type JsonBool::Type() const { return Json::JBOOL; }

bool JsonBool::BoolValue() const { return value_; }

void JsonBool::Dump(std::string* out) const {
  out->append(value_ ? "true" : "false");
}

class JsonString : public JsonValue {
 public:
  JsonString() {}
  explicit JsonString(const std::string& value) : value_(value) {}
  explicit JsonString(std::string&& value) : value_(std::move(value)) {}
  virtual const std::string& StringValue() const override;
  virtual Json::type Type() const;
  virtual void Dump(std::string* out) const override;

 private:
  std::string value_;
};

Json::type JsonString::Type() const { return Json::JSTRING; }

const std::string& JsonString::StringValue() const { return value_; }

// For string value, if the char is \\ or \" add a `\\` in front.
// if the char is \t, \b, \n, \f or \r, output the c-style char.
void JsonString::Dump(std::string* out) const { DumpString(value_, out); }

class JsonArray : public JsonValue {
 public:
  JsonArray() {}
  explicit JsonArray(const Json::Array& value) : value_(value) {}
  explicit JsonArray(Json::Array&& value) : value_(std::move(value)) {}
  // JsonArray(Json::Array&& value) : value_(std::move(value)) {}
  virtual void Dump(std::string* out) const;

  virtual const Json& operator[](size_t pos) const override;
  virtual const Json::Array& ArrayValue() const override;
  virtual Json::type Type() const;

 private:
  Json::Array value_;
};

Json::type JsonArray::Type() const { return Json::JARRAY; }

const Json::Array& JsonArray::ArrayValue() const { return value_; }

void JsonArray::Dump(std::string* out) const {
  bool add_comma = false;
  out->append(1, '[');
  for (const Json& json : value_) {
    if (add_comma) {
      out->append(", ");
    }
    json.GetJsonValue()->Dump(out);
    add_comma = true;
  }
  out->append(1, ']');
}

const Json& JsonArray::operator[](size_t pos) const { return value_[pos]; }

class JsonObject : public JsonValue {
 public:
  JsonObject() {}
  explicit JsonObject(const Json::Object& value) : value_(value) {}
  explicit JsonObject(Json::Object&& value) : value_(std::move(value)) {}
  virtual const Json& operator[](const std::string& key) const override;
  virtual const Json::Object& ObjectValue() const override;
  virtual Json::type Type() const;
  virtual void Dump(std::string* out) const;

 private:
  Json::Object value_;
};

Json::type JsonObject::Type() const { return Json::JOBJECT; }

const Json::Object& JsonObject::ObjectValue() const { return value_; }

const Json& JsonObject::operator[](const std::string& key) const {
  auto iter = value_.find(key);
  return iter == value_.end() ? default_json : iter->second;
}

void JsonObject::Dump(std::string* out) const {
  bool add_comma = false;
  out->append(1, '{');
  for (const auto& obj_pair : value_) {
    if (add_comma) {
      out->append(", ");
    }
    DumpString(obj_pair.first, out);
    out->append(": ");
    obj_pair.second.GetJsonValue()->Dump(out);
    add_comma = true;
  }
  out->append(1, '}');
}

bool JsonParser::Parse() {
  json_->clear();
  Json val = ParseInternal();
  json_->json_value_ = val.json_value_;
  if (json_->IsValid() && !json_data_.empty()) {
    char ch = NextCharacter();
    if (ch != 0) {
      json_->valid_ = false;
      json_->error_msg_.json_error_type = Json::END_ERROR;
    }
  }
  return json_->valid_;
}

Json JsonParser::ParseInternal() {
  Json json;
  // In case the parser already fails, return a empty json object.
  if (!json_->valid_) {
    return json;
  }
  char ch = NextCharacter();
  if (ch == 0) {  // set null if data is empty
    json.json_value_.reset(new JsonNull);
  } else if (ch == 't') {  // true
    if (json_data_.starts_with("true")) {
      json.json_value_.reset(new JsonBool(true));
      json_data_.remove_prefix(4);
    } else {
      Fail(Json::UNEXPECTED_CHAR, "true", json_data_.substr(0, 5));
    }
  } else if (ch == 'f') {  // false
    if (json_data_.starts_with("false")) {
      json.json_value_.reset(new JsonBool(false));
      json_data_.remove_prefix(5);
    } else {
      Fail(Json::UNEXPECTED_CHAR, "false", json_data_.substr(0, 6));
    }
  } else if (ch == '-' || std::isdigit(ch)) {  // number
    ParseNumber(&json);
  } else if (ch == 'n') {  // null
    if (json_data_.starts_with("null")) {
      json.json_value_.reset(new JsonNull);
      json_data_.remove_prefix(4);
    } else {
      Fail(Json::UNEXPECTED_CHAR, "null", json_data_.substr(0, 5));
    }
  } else if (ch == '"') {  // string
    ParseString(&json);
  } else if (ch == '[') {  // array
    json_data_.remove_prefix(1);
    Json::Array json_array;
    char ch = NextCharacter();
    while (ch != ']') {
      Json json_item = ParseInternal();
      if (!json_->IsValid()) {
        break;
      }
      json_array.push_back(json_item);
      ch = NextCharacter();
      if (ch == 0) {
        Fail(Json::UNEXPECTED_CHAR, ",", "");
        break;
      } else if (ch == ',') {
        json_data_.remove_prefix(1);
        ch = NextCharacter();
      } else if (ch != ']') {
        Fail(Json::UNEXPECTED_CHAR, "]", json_data_.substr(0, 1));
        break;
      }
    }
    json_data_.remove_prefix(1);
    json.json_value_.reset(new JsonArray(json_array));
  } else if (ch == '{') {  // object
  } else {
    Fail(Json::UNEXPECTED_CHAR, "null", json_data_.substr(0, 1));
  }
  if (json_->IsValid() && !IsNullOrSeparator(0)) {
    Fail(Json::UNEXPECTED_CHAR, "", json_data_.substr(0, 1));
  }
  return json;
}

void JsonParser::ConsumeWhiteSpace() {
  while (!json_data_.empty() && isspace(json_data_[0])) {
    json_data_.remove_prefix(1);
  }
}

char JsonParser::NextCharacter() {
  ConsumeWhiteSpace();
  return json_data_.empty() ? '\0' : json_data_[0];
}

bool JsonParser::IsHex(const std::string& number) const {
  if (number.size() != 4) {
    return false;
  } else {
    for (size_t i = 0; i < 4; i++) {
      const char ch = number[i];
      if (!isdigit(ch) && !('A' <= ch && ch <= 'F') &&
          !('a' <= ch && ch <= 'f')) {
        return false;
      }
    }
    return true;
  }
}

bool JsonParser::IsNullOrSeparator(size_t pos) const {
  size_t n = json_data_.size();
  return n <= pos || isspace(json_data_[pos]) || json_data_[pos] == ',' ||
         json_data_[pos] == ']' || json_data_[pos] == '}';
}

void JsonParser::Fail(Json::error_type error, std::string const& expect,
                      std::string const& actual) const {
  json_->valid_ = false;
  json_->error_msg_.json_error_type = error;
  json_->error_msg_.pos = length_ - json_data_.length();
  json_->error_msg_.actual = actual;
  json_->error_msg_.expect = expect;
}

bool JsonParser::ParseNumber(Json* json) {
  unsigned int integer_res = 0;
  bool is_negative = false;
  if (json_data_.starts_with("-")) {
    is_negative = true;
    json_data_.remove_prefix(1);
  }

  // Error: No digit for integer part.
  if (json_data_.empty() || !isdigit(json_data_[0])) {
    Fail(Json::INVALID_NUM, "", "");
    return false;
  }

  // NOTE: Be sure the json_data is not empty here.
  // Error: Leading zero error.
  if (json_data_.starts_with("0") && json_data_.size() > 1 &&
      isdigit(json_data_[1])) {
    Fail(Json::INVALID_NUM, "", "");
    return false;
  }

  while (!json_data_.empty() && isdigit(json_data_[0])) {
    integer_res *= 10;
    integer_res += json_data_[0] - '0';
    json_data_.remove_prefix(1);
  }

  // The number is only a integer.
  if (json_data_.empty() ||
      (json_data_[0] != 'e' && json_data_[0] != 'E' && json_data_[0] != '.')) {
    json->json_value_.reset(
        new JsonInt(is_negative ? -integer_res : integer_res));
    return true;
  }

  // Float number part.
  // NOTE: Be sure the json_data is not empty here.
  double float_res = integer_res;
  if (json_data_[0] == '.') {
    double point_weight = 0.1;
    json_data_.remove_prefix(1);
    // ERROR: No digit for decimal part.
    if (json_data_.empty() || !isdigit(json_data_[0])) {
      Fail(Json::INVALID_NUM, "", "");
      return false;
    }

    while (!json_data_.empty() && isdigit(json_data_[0])) {
      float_res += (json_data_[0] - '0') * point_weight;
      point_weight *= 0.1;
      json_data_.remove_prefix(1);
    }
  }

  // Non scientific notation float number.
  if (json_data_.empty() || (json_data_[0] != 'E' && json_data_[0] != 'e')) {
    json->json_value_.reset(
        new JsonDouble(is_negative ? -float_res : float_res));
    return true;
  }

  // Scientific notation float number.
  // NOTE: Be sure the json_data is not empty here.
  json_data_.remove_prefix(1);
  if (json_data_.empty()) {
    Fail(Json::INVALID_NUM, "", "");
    return false;
  }
  double base = 10;
  if (!json_data_.empty() && json_data_[0] == '+') {
    json_data_.remove_prefix(1);
  } else if (!json_data_.empty() && json_data_[0] == '-') {
    json_data_.remove_prefix(1);
    base = 0.1;
  }

  // ERROR: No digit for scientific notation part.
  if (json_data_.empty() || !isdigit(json_data_[0])) {
    Fail(Json::INVALID_NUM, "", "");
    return false;
  }

  double exponent = 0;
  while (!json_data_.empty() && isdigit(json_data_[0])) {
    exponent *= 10;
    exponent += (json_data_[0] - '0');
    json_data_.remove_prefix(1);
  }
  float_res *= pow(base, exponent);
  json->json_value_.reset(new JsonDouble(is_negative ? -float_res : float_res));
  return true;
}

bool JsonParser::ParseString(Json* json) {
  // Valid json string should start with “double quote”
  if (json_data_.empty() || json_data_[0] != '"') {
    return false;
  }
  json_data_.remove_prefix(1);

  std::string json_string;
  while (!json_data_.empty() && json_data_[0] != '"') {
    const char ch = json_data_[0];
    if (ch < 0x20) {
      Fail(Json::UNESCAPED_CTRL, "", "");
      return false;
    } else if (ch == '\\') {  // For Json trivial character.
      json_data_.remove_prefix(1);
      if (json_data_.empty()) {
        Fail(Json::BAD_ESCAPED, "", "");
        return false;
      } else if (json_data_[0] == 'u') {  // For unicode
        // TODO(ronaflx): Support other character beside BMP.
        json_data_.remove_prefix(1);
        std::string number = json_data_.substr(0, 4);
        if (IsHex(number)) {
          int32 hex_value = strtol(number.data(), nullptr, 16);
          EncodeUTF8(hex_value, &json_string);
          json_data_.remove_prefix(4);
        } else {
          Fail(Json::INVALID_ESCAPED, "", "");
          return false;
        }
      } else {  // For " \ and escaped character.
        size_t escape_id =
            std::find(kEscCharEnd, kEscCharEnd + kEscSize, json_data_[0]) -
            kEscCharEnd;
        if (escape_id != kEscSize) {
          json_string += kEscChar[escape_id];
          json_data_.remove_prefix(1);
        } else {
          Fail(Json::BAD_ESCAPED, "", "");
          return false;
        }
      }
    } else {  // For json non-trivial character.
      json_string += ch;
      json_data_.remove_prefix(1);
    }
  }

  // Valid json string should end with “double quote”
  if (json_data_.empty()) {
    Fail(Json::END_ERROR, "\"", "");
    return false;
  }
  json_data_.remove_prefix(1);
  json->json_value_.reset(new JsonString(json_string));
  return true;
}

}  // namespace json
