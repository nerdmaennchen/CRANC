#include <cranc/module/Module.h>
#include <cranc/protocol/PacketHandler.h>
#include <cranc/applicationConfig/ApplicationConfig.h>
#include <cranc/platform/system.h>
#include <string.h>

namespace {


class ApplicationConfigManager: public cranc::PacketHandler
{
public:
	ApplicationConfigManager() : cranc::PacketHandler(1) {}

	void handlePacket(cranc::PhyInterface* iface, uint8_t const* packet, uint32_t len) override {
		if (0 == len) {
			// return the description
			uint16_t totalPackeLen = 0;

			auto* handle0 = cranc::util::LinkedList<cranc::ApplicationConfigBase>::get().mFirst;
			auto* handle = handle0;
			while (handle) {
				totalPackeLen += 2;
				totalPackeLen += strlen(handle->getName())+1;
				totalPackeLen += strlen(handle->getFormat())+1;
				handle = handle->mNext;
			}
			iface->startPacket(mEPNum, totalPackeLen);

			handle = handle0;
			while (handle) {
				uint16_t paramLen = handle->getSize();
				iface->sendPacket(&paramLen, sizeof(paramLen));
				iface->sendPacket(handle->getName(), strlen(handle->getName())+1);
				iface->sendPacket(handle->getFormat(), strlen(handle->getFormat())+1);
				handle = handle->mNext;
			}
		} else if (1 == len) {
			auto* handle = cranc::util::LinkedList<cranc::ApplicationConfigBase>::get().mFirst;
			uint8_t idx = 0;
			uint8_t target = packet[0];
			while (handle) {
				if (target == idx) {
					uint16_t paramLen = handle->getSize();
					if (0 == paramLen) {
						// special case: if the handle does not have any size its a simple rpc
						handle->setValue(nullptr);
					} else {
						iface->startPacket(mEPNum, paramLen);
						iface->sendPacket(handle->getValue(), paramLen);
					}
					break;
				}
				++idx;
				handle = handle->mNext;
			}
		} else {
			// first byte is the index the rest is the content
			auto* handle = cranc::util::LinkedList<cranc::ApplicationConfigBase>::get().mFirst;
			uint8_t idx = 0;
			uint8_t target = packet[0];
			while (handle) {
				if (target == idx and len == handle->getSize() + 1U) {
					handle->setValue(&(packet[1]));
					break;
				}
				++idx;
				handle = handle->mNext;
			}
		}
	}
} appConfManager;

}

