
#include <cranc/module/Module.h>
#include <cranc/core/MessagePump.h>

// some functions need to be system provided (those are the functions needed to be portet/implemented!)
// Those function tend to be highly system specific

#include <mutex>
std::mutex g_mutex;
void system_mutex_lock() {
	g_mutex.lock();
}
void system_mutex_unlock() {
	g_mutex.unlock();
}


// here goes the main code
int main() {
	cranc::InitializeModules();
	cranc::MessagePump::get().run();
	return 0;
}
