#include <string>
#include <cstdarg>

static void StringAppendV(std::string* dst, const char* format, va_list ap) {
  static const int kSpaceLength = 1024;
  char space[kSpaceLength];
  va_list backup_ap;
  va_copy(backup_ap, ap);
  int result = vsnprintf(space, kSpaceLength, format, backup_ap);
  va_end(backup_ap);
  if (result < kSpaceLength) {
    if (result >= 0) {
      dst->append(space, result);
    }
    return;
  }

  // use a new char buffer to read again.
  int length = result + 1;
  char* buf = new char[length];
  va_copy(backup_ap, ap);
  result = vsnprintf(buf, length, format, backup_ap);
  va_end(backup_ap);

  if (result >= 0 && result < length) {
    dst->append(buf, result);
  }
  delete[] buf;
}

std::string StringPrintf(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string result;
  StringAppendV(&result, format, ap);
  va_end(ap);
  return result;
}

std::string& StringPrintf(std::string* dst, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  dst->clear();
  StringAppendV(dst, format, ap);
  va_end(ap);
  return *dst;
}

std::string& StringPrintfAppend(std::string* dst, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  StringAppendV(dst, format, ap);
  va_end(ap);
  return *dst;
}
