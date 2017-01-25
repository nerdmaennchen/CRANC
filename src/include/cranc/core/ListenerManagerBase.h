#pragma once

#include "MessageContainerBase.h"

namespace cranc
{

class ListenerManagerBase {
public:
	virtual void invoke(MessageContainerBase* msg) = 0;
};

}
