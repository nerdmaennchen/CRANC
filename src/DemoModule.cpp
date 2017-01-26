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
class TestModule1 : public cranc::Module, public cranc::Listener<int, 0>, public cranc::TimerCallback
{
	// this encapsulates a reference to a message and simultaneously
	// prevents anybody else from acquiring the underlying memory during "getFreeMessage"
	cranc::Message<int> mLastMsg;

	void callback() override { // callback when software timer ran out
		auto newMsg = cranc::getFreeMessage<int>();
		if (newMsg) {
			newMsg = mLastMsg + 1;
			newMsg.post<1>(); // post this message with id 1
		}
	}

	void callback(cranc::Message<int> const& msg) override { // callback to messages produced by TestModule2
		mLastMsg = msg;
	}

public:
	TestModule1(unsigned int level) : cranc::Module(level) {}

	void init(unsigned int) override {
		mLastMsg = cranc::getFreeMessage<int>();
		this->start(1000000, true); // run every second
	}
} testModule1(9);

class TestModule2 : public cranc::Listener<int, 1>
{
	cranc::Message<int> mLastMsg;
	void callback(cranc::Message<int> const& msg) override { // callback to messages produced by TestModule1
		mLastMsg = msg;
		auto newMsg = cranc::getFreeMessage<int>();
		if (newMsg) {
			newMsg = msg + 1;
			// call each callback registered to messages of type int with id 0 directly (without using the message pump)
			newMsg.invokeDirectly<0>();
		}
	}
} testModule2;

