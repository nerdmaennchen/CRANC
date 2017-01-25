#include <cranc/stdtypes.h>

#include <cranc/module/Module.h>

#include <cranc/core/Message.h>
#include <cranc/core/MessageBufferMemory.h>
#include <cranc/core/MessageBufferManager.h>

#include <cranc/core/Listener.h>
#include <cranc/timer/swTimer.h>

#include <cranc/applicationConfig/ApplicationConfig.h>


cranc::MessageBufferMemory<int, 5> intMsgBuf;

// two demoimplementations of modules that send messages to each other incrementing its value by one and remembering the last sent message
class TestModule1 : public cranc::Module, public cranc::Listener<int, 0>
{
	// this encapsulates a reference to a message and simultaneously
	// prevents anybody else from aquiring the underlying memory during "getFreeMessage"
	cranc::Message<int> mLastMsg;

	void callback(cranc::Message<int> const& msg) override {
		mLastMsg = msg;
		auto newMsg = cranc::getFreeMessage<int>();
		newMsg = msg + 1;
		newMsg.post<1>();
	}

public:
	TestModule1(unsigned int level) : cranc::Module(level) {}

	void init(unsigned int) override {
		auto msg = cranc::getFreeMessage<int>();
		if (msg) {
			msg = 0;
			msg.post<1>();
		}
	}
} testModule1(9);

class TestModule2 : public cranc::Listener<int, 1>
{
	cranc::Message<int> mLastMsg;
	void callback(cranc::Message<int> const& msg) override {
		mLastMsg = msg;
		auto newMsg = cranc::getFreeMessage<int>();
		newMsg = msg + 1;
		newMsg.post<0>();
	}
} testModule2;

