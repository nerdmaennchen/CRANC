/*
 * msg_time.c
 *
 *  Created on: 14.11.2011
 *      Author: lutz, simon
 */

#include <cranc/timer/swTimer.h>
#include <cranc/platform/hwTimer.h>

#include <algorithm>

constexpr uint32_t microSecondsPerSecond = 1000U;

namespace cranc
{

using HWTimer = cranc::platform::HWTimer;
void SWTimer::registerTimer(TimerCallback* cb)
{
	HWTimer& tim = HWTimer::get();
	cb->mTicksToGo = tim.getTicksForInterval_us(cb->mInterval);

	{
		LockGuard lock;
		cb->mNextCallback = mNextCallback;
		mNextCallback = cb;
	}

	// update all timers
	setupTimer();
}

void SWTimer::unregisterTimer(TimerCallback* cb)
{
	LockGuard lock;

	TimerCallback **iter = &mNextCallback;
	while (*iter) {
		TimerCallback **next = &((*iter)->mNextCallback);
		if (*iter == cb) {
			*iter = cb->mNextCallback;
			return;
		}
		iter = next;
	}
	setupTimer();
}

void SWTimer::setupTimer()
{
	LockGuard lock;
	timerInterval_t nextInterval = 0x7fffffff;

	TimerCallback *iter = mNextCallback;
	while (iter) {
		if (iter->mInterval != 0) {
			nextInterval = std::min(nextInterval, iter->mTicksToGo);
		}
		iter = iter->mNextCallback;
	}
	if (0x7fffffff != nextInterval)
	{
		/* just some minimal time */
		nextInterval = std::max(nextInterval, timerInterval_t(40));
		HWTimer::get().setupTimer(nextInterval);
	}
}

void SWTimer::trigger()
{
	LockGuard lock;

	HWTimer& tim = HWTimer::get();
	const HWTimer::hw_timerTicks ticksElapsed = tim.getTicksElapsed();
	TimerCallback *iter = mNextCallback;
	while (iter) {
		TimerCallback* next = iter->mNextCallback;
		if (iter->mTicksToGo > ticksElapsed) {
			iter->mTicksToGo -= ticksElapsed;
		} else {
			const HWTimer::hw_timerTicks ticks = tim.getTicksForInterval_us(iter->mInterval);
			const HWTimer::hw_timerTicks ticksOverdue = ticksElapsed - iter->mTicksToGo;
			iter->mTicksToGo = std::min(ticks, (ticks - ticksOverdue));
			iter->invoke();
		}
		iter = next;
	}
	setupTimer();
}

}
