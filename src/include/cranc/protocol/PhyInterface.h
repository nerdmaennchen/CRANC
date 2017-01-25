#pragma once

#include <cranc/util/LinkedList.h>
#include <cranc/stdtypes.h>

#include <cranc/protocol/PhyInterface.h>

namespace cranc
{

class PhyInterface : public cranc::util::LinkedListNode<PhyInterface> {
public:
	PhyInterface(uint8_t ifaceNo) : mIfaceNo(ifaceNo) {}

	virtual void startPacket(uint8_t, uint16_t) {}
	virtual void sendPacket(void const*, uint16_t) {}
	virtual void sendPacket(uint8_t epNum, void const* msg, uint16_t len) = 0;

	const uint8_t mIfaceNo;
};

}
