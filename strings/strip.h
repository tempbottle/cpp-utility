// Copyright 2014 ronaflx
#ifndef STRINGS_STRIP_H_
#define STRINGS_STRIP_H_

#include <string>
using std::string;

string Strip(const string& val, const string& skip_value);
string LeftStripChar(const string& val, const string& skip_value);
string RightStripChar(const string& val, const string& skip_value);
string StripWhiteSpace(const string& val);
string StripSpaceAndTab(const string& val);

#endif  // STRINGS_STRIP_H_
