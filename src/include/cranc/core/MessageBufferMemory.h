#pragma once

#include "MessageContainer.h"
#include "MessageBufferMemoryBase.h"
#include <cranc/util/Array.h>

namespace cranc
{

template<typename T, size_t N>
class MessageBufferMemory final : public MessageBufferMemoryBase<T> {
public:
	~MessageBufferMemory() {}

	MessageContainer<T>* getFreeMessage() override {
		cranc::LockGuard lock;
		for (size_t i(0); i < mMessages.size(); ++i) {
			MessageContainer<T> &msg = mMessages[i];
			if (not msg.locked()) {
				return &msg;
			}
		}
		return nullptr;
	}
protected:
	Array<MessageContainer<T>, N> mMessages;
};

}

