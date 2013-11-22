#ifndef UTIL_REGISTERER_H_
#define UTIL_REGISTERER_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include "base/macros.h"
using namespace std;

// T is a function pointer.
template<class T>
class Registerer {
 public:
  typedef T ObjectType;

  Registerer(const string& name, const string& filename, T object)
      : name_(name) {
    boost::call_once(module_init_, InitModule);
    pair<typename ObjectMap::iterator, bool> p = objects_->insert(
        make_pair(name, ObjectFilePair(object, filename)));
    if (!p.second) {
      ;
    }
  }

  ~Registerer() {
    typename ObjectMap::iterator iter = objects_->find(name_);
    if (iter != objects_->end()) {
      objects_->erase(iter);
    }
  }

  // Return the object function creator by name
  static T GetByName(const string& name) {
    boost::call_once(module_init_, InitModule);
    return GetObjectFilePairByName(name).first;
  }

  // Return the object file name by name
  static string GetFileNameByName(const string& name) {
    boost::call_once(module_init_, InitModule);
    return GetObjectFilePairByName(name).second;
  }

  static void GetNames(vector<string>* names) {
    boost::call_once(module_init_, InitModule);
    names->clear();
    for (const auto& i : *objects_) {
      names->push_back(i.first);
    }
    sort(names->begin(), names->end());
  }

 private:
  typedef pair<T, string> ObjectFilePair;
  typedef map<string, ObjectFilePair> ObjectMap;

  static void InitModule() {
    objects_ = new ObjectMap;
  }

  static const ObjectFilePair& GetObjectFilePairByName(const string& name) {
    boost::call_once(module_init_, InitModule);
    typename ObjectMap::const_iterator item = objects_->find(name);
    return item->second;
  }

  static boost::once_flag module_init_;
  static ObjectMap* objects_;
  const string name_;

  DISALLOW_COPY_AND_ASSIGN(Registerer);
};

template<class T>
boost::once_flag Registerer<T>::module_init_ = BOOST_ONCE_INIT;

template<class T>
typename Registerer<T>::ObjectMap* Registerer<T>::objects_ = NULL;

// R should be a ClassRegisterer
template<class R>
class RegistererWrapper {
 public:
  template<class N, class F>
  RegistererWrapper(const N& name, const F& file, typename R::Creator creator)
      : wrapped_(name, file, creator) {
  }

 private:
  R wrapped_;

  DISALLOW_COPY_AND_ASSIGN(RegistererWrapper);
};

template<class base>
class ClassRegisterer : public Registerer<base* (*)()> {
 public:
  typedef base CreateType;
  typedef CreateType* (*Creator)();
  ClassRegisterer(const string& name, const string& filename, Creator creator)
      : Registerer<Creator>(name, filename, creator) {
  }

  static CreateType* CreateByName(const string& name) {
    Creator creator = Registerer<Creator>::GetByName(name);
    return (*creator)();
  }

  template<class Type>
  static CreateType* Create() {
    return new Type();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(ClassRegisterer);
};

#define DEFINE_REGISTERER(ClassName)							                  \
  class ClassName##Registerer										                    \
      : public ClassRegisterer<ClassName> {							            \
   public:															                            \
    ClassName##Registerer(const string& name, const string& file,	  \
                          Creator creator)							            \
        : ClassRegisterer(name, file, creator) {}					          \
  };                                                                \
  typedef ClassName##Registerer::Creator ClassName##Creator

#define REGISTER_FACTORY_ENTITY(name, ClassName, FactoryName)   \
  static RegistererWrapper<ClassName##Registerer>               \
      __##name##__registerer(#name, __FILE__, FactoryName)

#define REGISTER_ENTITY(name, ClassName) \
  REGISTER_FACTORY_ENTITY(name, ClassName, ClassName##Registerer::Create<name>)

#endif  // UTIL_REGISTERER_H_

