#pragma once

#include <cranc/util/LinkedList.h>
#include "MessageBufferMemoryBase.h"
#include "Message.h"


namespace cranc
{

template<typename T>
Message<T> getFreeMessage() {
	cranc::LockGuard lock;
	static cranc::util::LinkedList<MessageBufferMemoryBase<T>> const& messageBufferMemoryManager = cranc::util::LinkedList<MessageBufferMemoryBase<T>>::get();
	MessageBufferMemoryBase<T>* buffer = messageBufferMemoryManager.mFirst;
	while (buffer) {
		MessageContainer<T> * msg = buffer->getFreeMessage();
		if (msg) {
			return Message<T>(msg);
		}
		buffer = buffer->mNext;
	}
	return Message<T>();
}

}

