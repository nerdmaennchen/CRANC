#pragma once

#include "MessageContainer.h"
#include "ListenerManagerBase.h"
#include "Listener.h"

namespace cranc
{

template<typename T, msgID_t msgID>
class ListenerManager final : public cranc::util::Singleton<ListenerManager<T, msgID>>, public ListenerManagerBase {
public:
	void invoke(MessageContainerBase* msg) override {
		Listener<T, msgID>* listener = cranc::util::LinkedList<Listener<T, msgID>>::get().mFirst;
		Message<T> typedMsg = Message<T>(reinterpret_cast<MessageContainer<T>*>(msg));
		while (listener) {
			listener->callback(typedMsg);
			listener = listener->mNext;
		}
	}
};

}
