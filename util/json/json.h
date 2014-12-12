// CopyRight 2014 ronaflx
#ifndef UTIL_JSON_JSON_H_
#define UTIL_JSON_JSON_H_
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "strings/string_piece.h"

namespace json {
class JsonValue;

// For json string, the output is unicode code point, but we store utf8 inside.
class Json {
  friend class JsonValue;
  friend class JsonParser;

 public:
  Json();
  // Define type for external use.
  enum type { JNUL, JINT, JREAL, JBOOL, JSTRING, JARRAY, JOBJECT };
  enum error_type {
    NO_ERROR,         // Parse successfully
    END_ERROR,        // Unexpected end of string.
    UNESCAPED_CTRL,   // Control character should be escaped while it's not.
    BAD_ESCAPED,      // Bad escaped character after '/'
    INVALID_ESCAPED,  // Invalid escaped character, except for bfnrt"\/
    INVALID_NUM,      // Parse number fail.
    UNEXPECTED_CHAR   // Unescaped character, typo for true false, null ':' ','
  };
  typedef std::vector<Json> Array;
  typedef std::map<std::string, Json> Object;
  struct ErrorMessage {
    size_t pos;
    error_type json_error_type;
    std::string expect;  // For some error stats, set expected value.
    std::string actual;  // For some error stats, set actual value.
  };

  const JsonValue* GetJsonValue() const;
  bool IsValid() const;
  void clear();

  // static function to parse json from string.
  // These will return false if parser fail.
  static bool ParseFromString(const char* json_data, size_t length, Json* json);
  static bool ParseFromString(const char* json_data, Json* json);
  static bool ParseFromString(const std::string& json_data, Json* json);

  // Member from to parse json from string.
  // These will return false if parser fail.
  bool Parse(const char* json_data, size_t length);
  bool Parse(const char* json_data);
  bool Parse(const std::string& json_data);

  // Serialize Json object to string without indent.
  std::string ToString() const;
  // Python like api for json oject in human readable way.
  // Python has dump, dumps, load and loads api.
  // I only use the s(string) version here.
  std::string Dump(int indent = 4) const;
  void Dump(std::string* out, int indent = 4) const;

  // Export JsonValue member to Json object.
  type Type() const;
  double RealValue() const;
  int IntValue() const;
  bool BoolValue() const;
  const std::string& StringValue() const;
  const Json::Array& ArrayValue() const;
  const Json::Object& ObjectValue() const;
  const Json& operator[](size_t pos) const;
  const Json& operator[](const std::string& key) const;

  // Function to report Parse error.
  error_type ErrorType() const;
  const ErrorMessage& ReportErrorMessage() const;

 private:
  // Use shared_ptr instead of unique_ptr, since unqiue_ptr don't allow copy.
  std::shared_ptr<JsonValue> json_value_;
  bool valid_;              // If the json is valid after parse.
  ErrorMessage error_msg_;  // The error message if the json object is invalid.

  // The return value for JsonValue type and called function dismatching.
  static const Json::Array default_array_;
  static const Json::Object default_object_;
  static const std::string default_string_;
};

class JsonValue {
 public:
  // For value access
  virtual Json::type Type() const = 0;
  virtual double RealValue() const;
  virtual int IntValue() const;
  virtual bool BoolValue() const;
  virtual const std::string& StringValue() const;
  virtual const Json::Array& ArrayValue() const;
  virtual const Json::Object& ObjectValue() const;

  // For member access of array and object.
  virtual const Json& operator[](size_t pos) const;
  virtual const Json& operator[](const std::string& key) const;

  // For Dump compressed text to string.
  virtual void Dump(std::string* out, bool break_line, int space,
                    int indent) const = 0;
};

class JsonNull;
class JsonDouble;
class JsonInt;
class JsonBool;
class JsonString;
class JsonArray;
class JsonObject;
}  // namespace json

#endif  // UTIL_JSON_JSON_H_
