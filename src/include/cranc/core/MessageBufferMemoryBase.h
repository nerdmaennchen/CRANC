#pragma once

#include <cranc/util/LinkedList.h>
#include "MessageContainer.h"

namespace cranc
{

template<typename T>
class MessageBufferMemoryBase : public cranc::util::LinkedListNode<MessageBufferMemoryBase<T>> {
protected:
	virtual ~MessageBufferMemoryBase() {}
public:
	virtual MessageContainer<T>* getFreeMessage() = 0;
};

}
