#pragma once

#include <cranc/stdtypes.h>
#include <cranc/util/Callback.h>
#include <cranc/util/LinkedList.h>
#include <string.h>

namespace cranc
{

class ApplicationConfigBase : public cranc::util::LinkedListNode<ApplicationConfigBase> {
public:
	virtual ~ApplicationConfigBase() {}

	virtual char const* getName() const = 0;
	virtual char const* getFormat() const = 0;

	virtual void setValue(void const* vals) = 0;
	virtual void const* getValue() = 0;
	virtual uint16_t getSize() const = 0;
};

template<typename T>
class ApplicationConfig final : public ApplicationConfigBase
{
	const char* name;
	const char* format;
	T value;
	using callbackType = Callback<T&, bool>;
	callbackType* mCB {nullptr};
public:

	ApplicationConfig(const char* _name, const char* _format) : name(_name), format(_format)  {}
	ApplicationConfig(const char* _name, const char* _format, callbackType* cb) : name(_name), format(_format), mCB(cb) {}
	ApplicationConfig(const char* _name, const char* _format, T const& val) : name(_name), format(_format), value(val) {}
	ApplicationConfig(const char* _name, const char* _format, callbackType* cb, T const& val) : name(_name), format(_format), value(val), mCB(cb) {}

	char const* getName() const override { return name; }
	char const* getFormat() const override { return format; }

	uint16_t getSize() const override {
		return sizeof(T);
	};

	ApplicationConfig<T>& operator=(T const& rhs) {
		value = rhs;
		return *this;
	}

	T& get() {
		return value;
	}

	operator T&() {
		return value;
	}

	operator T const&() const {
		return value;
	}

	T* operator->() {
		return &value;
	}

	void setValue(void const* vals) {
		memcpy(&value, vals, getSize());
		if (mCB) {
			mCB->callback(value, true);
		}
	}

	void const* getValue() override {
		if (mCB) {
			mCB->callback(value, false);
		}
		return &value;
	}
};

template<typename T>
class ApplicationConfigMapping final : public ApplicationConfigBase
{
	const char* name;
	const char* format;
	T& value;
	using callbackType = Callback<T&, bool>;
	callbackType* mCB {nullptr};
public:

	ApplicationConfigMapping(const char* _name, const char* _format, T& map) : name(_name), format(_format), value(map) {}
	ApplicationConfigMapping(const char* _name, const char* _format, callbackType* cb, T& map) : name(_name), format(_format), value(map), mCB(cb) {}

	char const* getName() const override { return name; }
	char const* getFormat() const override { return format; }

	uint16_t getSize() const override {
		return sizeof(T);
	};

	ApplicationConfig<T>& operator=(T const& rhs) {
		value = rhs;
		return *this;
	}

	T& get() {
		return value;
	}

	operator T&() {
		return value;
	}

	operator T const&() const {
		return value;
	}

	T* operator->() {
		return &value;
	}

	void setValue(void const* vals) {
		memcpy(&value, vals, getSize());
		if (mCB) {
			mCB->callback(value, true);
		}
	}

	void const* getValue() override {
		if (mCB) {
			mCB->callback(value, false);
		}
		return &value;
	}
};


template<>
class ApplicationConfig<void> final : public ApplicationConfigBase
{
	const char* name;
	const char* format;
	using callbackType = Callback<void>;
	callbackType* mCB {nullptr};
public:

	ApplicationConfig(const char* _name) : name(_name), format("") {}
	ApplicationConfig(const char* _name, callbackType* cb) : name(_name), format(""), mCB(cb) {}

	char const* getName() const override { return name; }
	char const* getFormat() const override { return format; }

	void const* getValue() override { return nullptr; };
	uint16_t getSize() const override { return 0; };

	void setValue(void const*) override {
		if (mCB) {
			mCB->callback();
		}
	}
};

} /* namespace cranc */

