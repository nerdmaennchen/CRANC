#pragma once

#include <cranc/module/Module.h>
#include <cranc/util/Singleton.h>

#include "Post.h"

namespace cranc
{

class MessagePump final : public cranc::util::Singleton<MessagePump>
{
public:
	bool dispatch(Post post);

	void run();
};

} /* namespace cranc */
