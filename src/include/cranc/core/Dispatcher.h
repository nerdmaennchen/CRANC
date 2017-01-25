#pragma once

#include <cranc/util/Singleton.h>
#include "Listener.h"

namespace cranc
{

template<typename T, msgID_t msgID>
class Dispatcher : public cranc::util::Singleton<Dispatcher<T, msgID>> {
};

}
