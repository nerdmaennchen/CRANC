#pragma once

#include <cranc/util/function.h>
#include "cranc/platform/system.h"
#include "cranc/msg/Message.h"

#include <coroutine>
#include <optional>
#include <cassert>

namespace cranc::coro
{

struct SwitchToMainLoop {
    bool await_ready();
    
    void await_suspend(std::coroutine_handle<> h) {
        auto msg = msgBuf.getFreeMessage();
        assert(msg);
        {
            cranc::LockGuard lock;
            (*msg)->instance = this;
            ++ticker;
            (*msg)->val = ticker;
            expected_val = ticker;
        }
        handle = h; 
        msg->post();
    }
    void await_resume() { handle = nullptr; }

    void remove_awaiter() {
        // does this need to be implemented?
        handle = nullptr;
    }

    ~SwitchToMainLoop(){}
private:
    inline static std::size_t ticker{};
    std::size_t expected_val{};

    std::coroutine_handle<> handle{};
    struct SwitchMsg{ SwitchToMainLoop* instance; std::size_t val; };
	cranc::MessageBufferMemory<SwitchMsg, 2> msgBuf;
    cranc::Listener<SwitchMsg> trampoline {[this](auto const& msg) {
        if (msg.instance == this and msg.val == expected_val) {
            handle.resume();
        }
    }};
};

}
