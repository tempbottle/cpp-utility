// Copyright 2014 ronaflx
#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

// disallow copy an assign in class, use in private field.
#if LANG_CXX11 && !defined(__clang__)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;    \
  void operator=(const TypeName&) = delete
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
  void operator=(const TypeName&)
#endif

// this is better than use two sizeof(array) / sizeof(array[0]).
template<typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// for debug info.
// the reason I use two macro here.
// #define ABC CBA
// AS_STRING(ABC)         --> "CBA"
// AS_STRINGINTERNAL(ABC) --> "ABC"
#define AS_STRING(x)   AS_STRING_INTERNAL(x)
#define AS_STRING_INTERNAL(x)   #x

#endif  // BASE_MACROS_H_
