// Copyright 2014 ronaflx
// Implementation of cookie base on RFC6265
#ifndef UTIL_WEB_COOKIE_H_
#define UTIL_WEB_COOKIE_H_
#include <sys/time.h>
#include <string>

using std::string;
namespace web {

class Cookie {
 public:
  static const size_t kCookieMaxSize = 4096;
  Cookie();
  virtual ~Cookie();

  const string& get_domain() const {
    return domain_;
  }

  void set_domain(const string& domain) {
    domain_ = domain;
  }

  time_t get_expires() const {
    return expires_;
  }

  void set_expires(time_t expires) {
    expires_ = expires;
  }

  bool is_http_only() const {
    return http_only_;
  }

  void set_http_only(bool httpOnly) {
    http_only_ = httpOnly;
  }

  const string& get_max_age() const {
    return max_age_;
  }

  void set_max_age(const string& maxAge) {
    max_age_ = maxAge;
  }

  const string& get_path() const {
    return path_;
  }

  void set_path(const string& path) {
    path_ = path;
  }

  bool is_secure() const {
    return secure_;
  }

  void set_secure(bool secure) {
    secure_ = secure;
  }

  // Set-Cookie header when server sending a http request to user agent.
  // Based on Section 4.1.
  bool ParseSetCookieHeader(const string& header);
  string WriteSetCookieHeader() const;
  // Cookie header when user agent send stored cookie to the server.
  // Based on Section 4.2.
  bool ParseCookieHeader(const string& header);
  string WriteCookieHeader() const;

  const string& get_name() const {
    return name_;
  }

  void set_name(const string& name) {
    name_ = name;
  }

  const string& get_value() const {
    return value_;
  }

  void set_value(const string& value) {
    value_ = value;
  }

 private:
  string name_;
  string value_;

  // Attribute of cookie based on section 5.2
  time_t expires_;  // 5.2.1
  string max_age_;  // 5.2.2
  string domain_;   // 5.2.3
  string path_;     // 5.2.4
  bool secure_;     // 5.2.5
  bool http_only_;  // 5.2.6
};

}  // namespace web

#endif  // UTIL_WEB_COOKIE_H_
