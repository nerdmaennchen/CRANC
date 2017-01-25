#pragma once

#include <cranc/stdtypes.h>
#include <cranc/util/LinkedList.h>
#include <cranc/protocol/PhyInterface.h>

namespace cranc
{

class PacketHandler : public cranc::util::LinkedListNode<PacketHandler> {
public:
	PacketHandler(uint8_t epNum) : mEPNum(epNum) {}

	virtual void handlePacket(PhyInterface* iface, uint8_t const* data, uint32_t len) = 0;

	const uint8_t mEPNum;
};
}
