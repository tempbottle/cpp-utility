// Copyright 2014 ronaflx
#ifndef STRINGS_STRINGPRINTF_H_
#define STRINGS_STRINGPRINTF_H_
#include <string>

extern std::string StringPrintf(const char* format, ...);

extern std::string& StringPrintf(std::string* dst, const char* format, ...);

extern std::string& StringPrintfAppend(std::string* dst, const char* format,
                                       ...);
#endif  // STRINGS_STRINGPRINTF_H_
