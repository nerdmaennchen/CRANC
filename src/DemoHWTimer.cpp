
#include <cranc/platform/hwTimer.h>
#include <cranc/timer/swTimer.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>

namespace cranc
{
namespace platform
{

namespace {
struct TimerWorker {
	std::thread mThread;
	std::mutex mMutex;
    std::condition_variable cv;
    std::atomic<bool> stopFlag{false};

    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

    TimerWorker() {
    	mThread = std::thread([=]() {
			while (not stopFlag) {
				{
					std::unique_lock<std::mutex> lock(mMutex);
					if (cv.wait_until(lock, endTime, [=]() {return stopFlag == true;})) {
						break;
					}
				}
				cranc::SWTimer::get().trigger();
			}
    	});
    }

    ~TimerWorker() {
		stopFlag = true;
    	cv.notify_one();
		mThread.join();
    }

    void run(HWTimer::hw_timerTicks ticks) {
		std::unique_lock<std::mutex> lock(mMutex);
    	startTime = std::chrono::steady_clock::now();
    	endTime = startTime + std::chrono::microseconds(ticks);
    	cv.notify_one();
    }

    HWTimer::hw_timerTicks getTicksElapsed() {
		std::unique_lock<std::mutex> lock(mMutex);
    	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count();
    }
} gTimerWorker;
}

HWTimer::hw_timerTicks HWTimer::getTicksForInterval_us(HWTimer::timer_TimeInterval_us interval) {
	return interval;
}

HWTimer::hw_timerTicks HWTimer::getTicksElapsed() {
	return gTimerWorker.getTicksElapsed();
}

void HWTimer::setupTimer(HWTimer::hw_timerTicks ticks) {
	gTimerWorker.run(ticks);
}


}
}
