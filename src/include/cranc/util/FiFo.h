#pragma once

#include "Array.h"
#include <algorithm>

namespace cranc
{

template<typename T, size_t N, typename Lock = int>
class FIFO final {
public:
	size_t put(T const& _val) {
		Lock __attribute__((unused)) lock;
		if (mCount < N) {
			mStorage[mEnd] = _val;
			mEnd = (mEnd + 1) % N;
			++mCount;
			return 1;
		}
		return 0;
	}

	size_t put(T const* _vals, size_t cnt) {
		Lock __attribute__((unused)) lock;
		size_t p = 0;
		while (p < cnt && put(_vals[p])) {
			++p;
		}
		return p;
	}

	size_t peek(T const*& target) {
		Lock __attribute__((unused)) lock;
		target = nullptr;
		if (mCount > 0) {
			target = &(mStorage[mStart]);
			size_t endIdx = std::min(mStart + mCount, N);
			return endIdx - mStart;
		}
		return 0;
	}

	void pop(size_t cnt) {
		Lock __attribute__((unused)) lock;
		if (cnt <= mCount) {
			mStart = (mStart + cnt) % N;
			mCount -= cnt;
		} else {
			clear();
		}
	}

	void clear() {
		Lock __attribute__((unused)) lock;
		mStart = 0;
		mEnd = 0;
		mCount = 0;
	}

	T& operator[](size_t idx) {
		idx = (mStart + idx) % N;
		return mStorage[idx];
	}

	size_t countFree() const {
		return N - mCount;
	}

	size_t count() const {
		return mCount;
	}

	constexpr size_t capacity() {
		return N;
	}

private:
	volatile size_t mCount {0}, mStart {0}, mEnd {0};
	Array<T, N> mStorage;
};

}
