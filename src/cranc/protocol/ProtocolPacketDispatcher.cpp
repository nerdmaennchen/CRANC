/*
 * ProtocolPacketDispatcher.cpp
 *
 *  Created on: 04.09.2016
 *      Author: lutz
 */

#include <cranc/protocol/ProtocolPacketDispatcher.h>
#include <cranc/protocol/PacketHandler.h>
#include <cranc/module/Module.h>

namespace {

cranc::MessageBufferMemory<cranc::Packet_, 10> packetMessageBuffer;

class : public cranc::Listener<cranc::Packet_> {
	void callback(cranc::Packet const& packet) override {
		if (packet->len > 0) {
			uint8_t ep = packet->buffer[0];

			auto* handler = cranc::util::LinkedList<cranc::PacketHandler>::get().mFirst;
			while (handler) {
				if (handler->mEPNum == ep) {
					handler->handlePacket(packet->iface, &(packet->buffer[1]), packet->len - 1);
				}
				handler = handler->mNext;
			}
		}
	}
} pcketListener;


}
