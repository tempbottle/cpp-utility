cpp-utility-tool
================

some useful tool for cpp

##C++ registerer Class
`util/registerer.h`

	// define a base class
	class Builder {
    	public:
    	virtual void builder(const char* sentence) = 0;
    	virtual ~Builder(){}
  	};
  	DEFINE_REGISTERER(Builder);
  	#define REGISTER_BUILDER(name) REGISTER_ENTITY(name, Builder)
  	
  	// define a builder
  	class TermBuilder : public Builder {
  		public:
  		virtual void builder(const char* sentence) {
  			// your code here.
  		}
  	};
  	REGISTER_BUILDER(TermBuilder);
  	
##C++ singleton pattern
`util/singleton.h`
	
	// get the singleton instance directly.
	Singleton<Type>::Get();
	// get const reference directly.
	Singleton<Type>::GetReg();
	// thread unsafe reset
	Singleton<Type>::UnsafeReset();
