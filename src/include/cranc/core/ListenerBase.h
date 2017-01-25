#pragma once


namespace cranc
{

class ListenerBase {
public:
	virtual void invoke() = 0;

protected:
	virtual ~ListenerBase() {}
};

}
