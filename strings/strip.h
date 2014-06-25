// Copyright 2014 ronaflx
#ifndef STRINGS_STRIP_H_
#define STRINGS_STRIP_H_

#include <string>
using std::string;

extern string Strip(const string& val, const string& skip_value);
extern string LeftStripChar(const string& val, const string& skip_value);
extern string RightStripChar(const string& val, const string& skip_value);
extern string StripWhiteSpace(const string& val);
extern string StripSpaceAndTab(const string& val);

#endif  // STRINGS_STRIP_H_
