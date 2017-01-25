#pragma once

#include "MessageContainerBase.h"

namespace cranc
{

using msgID_t = int;

template<typename T>
class MessageContainer final : public MessageContainerBase
{
public:
	MessageContainer() : mMessage() {}

	T& operator=(T const& rhs) {
		return mMessage = rhs;
	}
	T& operator*() {
		return mMessage;
	}

	T& operator->() {
		return mMessage;
	}

	T* get() {
		return &mMessage;
	}

private:
	T mMessage;
};

}

