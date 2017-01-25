#pragma once

namespace cranc
{
template<typename ... Args>
class Callback
{
public:
	virtual void callback(Args ...) = 0;

protected:
	virtual ~Callback() {}
};

template<>
class Callback<void>
{
public:
	virtual void callback() = 0;

protected:
	virtual ~Callback() {}
};

template<typename ... Args>
class RefCallback
{
public:
	virtual void callback(Args const& ...) = 0;

protected:
	virtual ~RefCallback() {}
};

}
