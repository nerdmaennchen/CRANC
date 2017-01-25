#pragma once

#include "MessageContainerBase.h"
#include "ListenerManagerBase.h"

namespace cranc
{

class MessageBufferBase;
class Post final {
public:
	Post() {}
	Post(MessageContainerBase* msg, ListenerManagerBase* msgBuffer) :
		mMsg(msg),
		mListenerManager(msgBuffer)
	{
		mMsg->lock();
	}

	~Post() {
		if (mMsg) {
			mMsg->unlock();
		}
	}

	Post const& operator=(Post const& rhs) {
		mMsg = rhs.mMsg;
		mListenerManager = rhs.mListenerManager;
		mMsg->lock();
		return (*this);
	}

 	void invoke() {
 		mListenerManager->invoke(mMsg);
		mMsg->unlock();
	}

protected:
	MessageContainerBase *mMsg {nullptr};
	ListenerManagerBase* mListenerManager {nullptr};
};
}
