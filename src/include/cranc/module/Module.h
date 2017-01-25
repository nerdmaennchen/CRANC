#pragma once

#include <cranc/util/LinkedList.h>

namespace cranc
{


class Module : public cranc::util::LinkedListNode<Module> {
public:
	Module(unsigned int initLevel) : mInitLevel(initLevel) {}
	virtual ~Module() {}
	virtual void init(unsigned int level) = 0;

	const unsigned int mInitLevel;
};

void InitializeModules();

}

