#pragma once

#include <cranc/platform/system.h>

namespace cranc
{
namespace util
{

template<class T>
class Singleton {
public:
	static T& get() {
		LockGuard lock;
		static T instance;
		return instance;
	}
protected:
	~Singleton() {}
};

}
}

