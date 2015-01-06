// Copyright 2014 ronaflx
#ifndef UTIL_REGISTERER_H_
#define UTIL_REGISTERER_H_

#include <algorithm>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "base/macros.h"

using std::map;
using std::pair;
using std::string;
using std::vector;

// T is a std::function pointer.
template <class T>
class Registerer {
 public:
  typedef T ObjectType;

  Registerer(const string& name, const string& filename, T* object)
      : name_(name) {
    std::call_once(module_init_, InitModule);
    pair<typename ObjectMap::iterator, bool> p =
        objects_->insert(make_pair(name, ObjectFilePair(object, filename)));
    if (!p.second) {
      // report error here.
    }
  }

  ~Registerer() {
    typename ObjectMap::iterator iter = objects_->find(name_);
    if (iter != objects_->end()) {
      objects_->erase(iter);
    }
  }

  // Return the object function creator by name
  static T* GetByName(const string& name) {
    std::call_once(module_init_, InitModule);
    return GetObjectFilePairByName(name).first;
  }

  // Return the object file name by name
  static string GetFileNameByName(const string& name) {
    std::call_once(module_init_, InitModule);
    return GetObjectFilePairByName(name).second;
  }

  static void GetNames(vector<string>* names) {
    std::call_once(module_init_, InitModule);
    names->clear();
    for (const auto& i : *objects_) {
      names->push_back(i.first);
    }
    sort(names->begin(), names->end());
  }

 private:
  typedef pair<T*, string> ObjectFilePair;
  typedef map<string, ObjectFilePair> ObjectMap;

  static void InitModule() { objects_ = new ObjectMap; }

  static const ObjectFilePair& GetObjectFilePairByName(const string& name) {
    std::call_once(module_init_, InitModule);
    typename ObjectMap::const_iterator item = objects_->find(name);
    return item->second;
  }

  static std::once_flag module_init_;
  static ObjectMap* objects_;
  const string name_;

  DISALLOW_COPY_AND_ASSIGN(Registerer);
};

template <class T>
std::once_flag Registerer<T>::module_init_;

template <class T>
typename Registerer<T>::ObjectMap* Registerer<T>::objects_ = NULL;

// R should be a ClassRegisterer
template <class R>
class RegistererWrapper {
 public:
  template <class N, class F>
  RegistererWrapper(const N& name, const F& file,
                    typename R::CreatorFunctionPtr creator)
      : wrapped_(name, file, creator) {}

  template <class N, class F>
  RegistererWrapper(const N& name, const F& file, typename R::Creator* creator)
      : wrapped_(name, file, creator) {}

 private:
  R wrapped_;

  DISALLOW_COPY_AND_ASSIGN(RegistererWrapper);
};

template <class base>
class ClassRegisterer : public Registerer<std::function<base*()>> {
 public:
  typedef base CreateType;
  typedef std::function<CreateType*()> Creator;
  ClassRegisterer(const string& name, const string& filename, Creator* creator)
      : Registerer<Creator>(name, filename, creator) {}

  typedef CreateType* (*CreatorFunctionPtr)();
  ClassRegisterer(const string& name, const string& filename,
                  CreatorFunctionPtr creator)
      : Registerer<Creator>(name, filename, new Creator(creator)) {}

  static CreateType* CreateByName(const string& name) {
    Creator* creator = Registerer<Creator>::GetByName(name);
    return (*creator)();
  }

  template <class Type>
  static CreateType* Create() {
    return new Type();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ClassRegisterer);
};

#define DEFINE_REGISTERER(ClassName)                                \
  class ClassName##Registerer : public ClassRegisterer<ClassName> { \
   public:                                                          \
    ClassName##Registerer(const string& name, const string& file,   \
                          CreatorFunctionPtr creator)               \
        : ClassRegisterer(name, file, creator) {}                   \
    ClassName##Registerer(const string& name, const string& file,   \
                          Creator* creator)                         \
        : ClassRegisterer(name, file, creator) {}                   \
  };                                                                \
  typedef ClassName##Registerer::Creator ClassName##Creator

#define REGISTER_FACTORY_ENTITY(name, ClassName, FactoryName)             \
  static RegistererWrapper<ClassName##Registerer> __##name##__registerer( \
      #name, __FILE__, FactoryName)

#define REGISTER_ENTITY(name, ClassName) \
  REGISTER_FACTORY_ENTITY(name, ClassName, ClassName##Registerer::Create<name>)

#endif  // UTIL_REGISTERER_H_

