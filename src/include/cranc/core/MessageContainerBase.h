#pragma once

#include <cranc/platform/system.h>

namespace cranc
{


class MessageContainerBase {
protected:
	using referenceCountType_t = int;
public:
	bool locked() const {
		cranc::LockGuard lock;
		return mReferenceCount != 0;
	}
	void lock() {
		cranc::LockGuard lock;
		++mReferenceCount;
	}
	void unlock() {
		cranc::LockGuard lock;
		--mReferenceCount;
	}

protected:
	virtual ~MessageContainerBase() {}
	referenceCountType_t mReferenceCount {0};
};

}
