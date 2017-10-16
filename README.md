# CRANC
Completely Random AcroNym CRANC

CRANC is a super lightweight c++ framework for use in embedded systems.
It provides features as message passing, ordered module initialization, protocol abstraction, easy to use software-timers and some more!

CRANC is what happens if you think about using C++ on microcontrollers... 
Is there actually any reason not to do that?
Just think about template use for all the crappy stuff people used to implement via linker magic or defines!


The framework is built around the idea that different functionalities should live in different c++ files and should not know about the existence of other functionalities.
This means that the only need for header files is then to define the content of messages (aka interfaces).

Summed up the amount of code necessary to implement a somewhat complex amount of features on an embedded system should be minimized when using this framework, easy to debug and readable.

If you want to use this framework you need to implement certain platform dependent functions:
```c++
void system_mutex_lock(void); // lock a global recursive mutex (usually a suspend\_all\_interrupts with lock counting equivalent)
void system_mutex_unlock(void); // the ovious counterpart
```

If you want to use features like communication though the interface you need to instanciate any subclass of cranc::PhyInterface that can receive and send packetized.
On the receiving side this subclass usually implements something like:

```c++
auto packet = cranc::getFreeMessage<cranc::Packet_>(); // memory to store an incomming packet in 
if (packet) { // if that memory was aquired
	packet->iface = this; // tell the framewok which interface received the packet
	packet->buffer = YourData; // fill the content of the message
	packet->len = lengthOfReceivedPacket; 
	packet.post();
}
```

To enable software timers you need to implement the class HWTimer in the fashion of your platform.
That involves conversion from microseconds to ticks (whatever ticks mean in your context... e.g., hw-timer-ticks) and vice versa.
Also the currently elapsed amount of ticks has to be provided (usually by providing the content of the timer count register).
The framework calls `setupTimer(hw_timerTicks i_ticks)` to start the timer with a given amount of ticks and expects a call to `cranc::SWTimer::get().trigger();` within the ISR when the timer overflowed.


And that is it!
The framework does not need any more porting to supply you with software timers, a generic communication interface (to implement getters, setters, rpc via instances of `cranc::ApplicationConfig`), an ordered initialization scheme and message passing!

Did I mention, that the output binary is _super_ small?
