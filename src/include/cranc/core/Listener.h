#pragma once

#include "Message.h"
#include <cranc/util/LinkedList.h>
#include <cranc/util/Callback.h>

namespace cranc
{

template<typename T, msgID_t msgID=0>
class Listener : public cranc::util::LinkedListNode<Listener<T, msgID>>, public cranc::RefCallback<Message<T>>
{};

} /* namespace cranc */

