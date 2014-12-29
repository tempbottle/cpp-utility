// Copyright 2014 ronaflx
#include "util/web/cookie.h"

#include <string>

#include "strings/stringprintf.h"
namespace web {

Cookie::Cookie()
    : expires_(-1),
      secure_(false),
      http_only_(false) {
}

Cookie::~Cookie() {
}

string Cookie::WriteSetCookieHeader() const {
  if (name_.size() + value_.size() > kCookieMaxSize) {
    return "";
  }
  string res_value;
  res_value.resize(kCookieMaxSize);
  StringPrintfAppend(&res_value, "%s=%s", name_.c_str(), value_.c_str());
  if (expires_ >= 0) {
    char time_buffer[32];
    struct tm tmbuf;
    strftime(time_buffer, sizeof(time_buffer), "%a, %d-%b-%Y %H:%M:%S GMT",
             gmtime_r(&expires_, &tmbuf));  // rfc1123-date
    if (res_value.size() + 10 + strlen(time_buffer) < kCookieMaxSize)
      StringPrintfAppend(&res_value, "; Expires=%s", time_buffer);
  }
  if (max_age_ != ""
      && res_value.size() + 10 + max_age_.size() < kCookieMaxSize) {
    StringPrintfAppend(&res_value, "; Max-Age=%s", max_age_.c_str());
  }

  if (domain_ != "" && res_value.size() + 9 + domain_.size() < kCookieMaxSize) {
    StringPrintfAppend(&res_value, "; Domain=%s", domain_.c_str());
  }

  if (path_ != "" && res_value.size() + 7 + path_.size() < kCookieMaxSize) {
    StringPrintfAppend(&res_value, "; Path=%s", path_.c_str());
  }

  if (secure_ && res_value.size() + 6 < kCookieMaxSize) {
    StringPrintfAppend(&res_value, "; Secure");
  }
  if (http_only_ && res_value.size() + 9 < kCookieMaxSize) {
    StringPrintfAppend(&res_value, "; HttpOnly");
  }

  return res_value;
}

}  // namespace web
