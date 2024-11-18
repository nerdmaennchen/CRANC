#include "SwitchToMainLoop.h"

#include <hardware/structs/scb.h>

namespace cranc::coro
{

bool SwitchToMainLoop::await_ready() { 
    auto running_from_main = (scb_hw->icsr & M0PLUS_ICSR_VECTACTIVE_BITS) == 0;
    return running_from_main;
}

}