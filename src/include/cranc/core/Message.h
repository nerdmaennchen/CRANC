#pragma once

#include "MessageContainer.h"
#include "MessagePump.h"
#include <cranc/platform/system.h>

namespace cranc
{


template<typename T>
class Message final {
public:
	Message() {}
	Message(MessageContainer<T>* msg) : mMsg(msg) {
		if (mMsg) {
			mMsg->lock();
		}
	}

	~Message() {
		if (mMsg) {
			mMsg->unlock();
		}
	}

	operator bool() const {
		return mMsg;
	}
	operator T() const {
		return *(mMsg->get());
	}

	T* operator->() {
		return mMsg->get();
	}
	T const* operator->() const {
		return mMsg->get();
	}

	Message<T>& operator=(Message<T> const& rhs) {
		rhs.mMsg->lock();
		if (mMsg) {
			mMsg->unlock();
		}
		mMsg = rhs.mMsg;
		return *this;
	}

	Message<T>& operator=(T const& rhs) {
		*mMsg = rhs;
		return *this;
	}

	MessageContainer<T>* get() {
		return mMsg;
	}

	template<msgID_t msgID = 0>
	void post();

	template<msgID_t msgID = 0>
	void invokeDirectly();

private:
	MessageContainer<T>* mMsg {nullptr};
};

}

#include "ListenerManager.h"

namespace cranc
{
template<typename T>
template<msgID_t msgID>
void Message<T>::post()
{
	cranc::LockGuard lock;

	if (not mMsg) {
		return;
	}

	cranc::ListenerManagerBase *listenerManager = &(cranc::ListenerManager<T, msgID>::get());
	bool dispatchSuccess = cranc::MessagePump::get().dispatch(Post(mMsg, listenerManager));
	if (dispatchSuccess) {
		mMsg->unlock();
		mMsg = nullptr;
	} else {
		return;
	}
}

template<typename T>
template<msgID_t msgID>
void Message<T>::invokeDirectly()
{
	if (not mMsg) {
		return;
	}
	cranc::ListenerManager<T, msgID>::get().invoke(mMsg);
}


}
