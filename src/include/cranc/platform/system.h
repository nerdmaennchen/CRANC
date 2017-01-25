#pragma once

#include <cranc/stdtypes.h>

/*
 * here are the most important platform dependant functions listed.
 * The framework cannot work correctly if those functions are not implemented correctly!
 */


/*
 * mutex functions (eg. suspend all interrupts)
 * The mutex functions must be safe for recursive usage!
 */
void system_mutex_lock(void);
void system_mutex_unlock(void);
void system_reset(void);

namespace cranc
{

class LockGuard final {
public:
	LockGuard() {
		system_mutex_lock();
	}
	~LockGuard() {
		system_mutex_unlock();
	}
};

}
