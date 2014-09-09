// Copyright 2014 ronaflx

#include "strings/strip.h"
#include <string>

string Strip(const string& val, const string& skip_value) {
  string::size_type start = val.find_first_not_of(skip_value);
  string::size_type end = val.find_last_not_of(skip_value);
  if (start == string::npos || end == string::npos) {
    return "";
  } else {
    string::size_type len = end - start + 1;
    return val.substr(start, len);
  }
}

string LeftStripChar(const string& val, const string& skip_value) {
  string::size_type pos = val.find_first_not_of(skip_value);
  if (pos == string::npos) {
    return "";
  } else {
    return val.substr(pos);
  }
}

string RightStripChar(const string& val, const string& skip_value) {
  string::size_type pos = val.find_last_not_of(skip_value);
  if (pos == string::npos) {
    return "";
  } else {
    return val.substr(0, pos);
  }
}

string StripWhiteSpace(const string& val) {
  return Strip(val, " \t\n\v\f\r");
}

string StripSpaceAndTab(const string& val) {
  return Strip(val, " \t");
}
