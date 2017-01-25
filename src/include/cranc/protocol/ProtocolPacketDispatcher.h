#pragma once

#include <cranc/stdtypes.h>
#include <cranc/core/MessageBufferMemory.h>
#include <cranc/core/MessageBufferManager.h>
#include <cranc/core/Message.h>
#include <cranc/config/ProtocolConfig.h>
#include <cranc/util/Array.h>

#include <cranc/protocol/PhyInterface.h>

namespace cranc
{

struct Packet_ {
	PhyInterface *iface;
	Array<uint8_t, MaxPacketLength> buffer;
	size_t len;
};
using Packet = cranc::Message<Packet_>;


} /* namespace cranc */

