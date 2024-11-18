#include "cranc/msg/MessagePump.h"
#include "cranc/platform/system.h"
#include "cranc/util/FiFo.h"

#include "cranc/timer/ISRTime.h"

namespace cranc
{

namespace {

auto& queue = cranc::util::GloballyLinkedList<MessageBase>::getHead();

}

MessageBase* MessagePump::popMessage() {
	cranc::LockGuard lock;
	if (queue.empty()) {
		return {};
	}

	auto* first = queue.next;
	first->remove();
	return static_cast<MessageBase*>(first);
}


}
