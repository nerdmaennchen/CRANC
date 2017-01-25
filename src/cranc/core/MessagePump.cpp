#include <cranc/core/MessagePump.h>
#include <cranc/platform/system.h>
#include <cranc/config/MessagePumpConfig.h>
#include <cranc/util/Array.h>
#include <cranc/util/FiFo.h>

namespace cranc
{

namespace {
	FIFO<Post, MessagePumpQueueSize, LockGuard> gMsgQueue;
}

bool MessagePump::dispatch(Post post)
{
	return gMsgQueue.put(post);
}


void MessagePump::run()
{
	while (true)
	{
		if (gMsgQueue.count()) {
			Post& head = gMsgQueue[0];
			head.invoke();
			gMsgQueue.pop(1);
		}
	}
}

} /* namespace cranc */
