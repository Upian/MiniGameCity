#include "SocialPacket.h"

char* ChatAllRequest::Serialize() {
	int basePacketTypeLength = sizeof(GetBasePacketType());
	int socialPacketTypeLength = sizeof(GetSocialPacketType());
	int chatAllPacketLength = sizeof(GetChatAllRequest()) + 1;
	char* buf = new char[basePacketTypeLength + socialPacketTypeLength + chatAllPacketLength];
	char* bufInit = buf;
	TypeSerial(buf, GetBasePacketType());
	TypeSerial(buf, GetSocialPacketType());
	StringSerial(buf, GetChatAllRequest());
	return bufInit;
}

void ChatAllRequest::Deserialize(char* _buf) {
	if (_buf == nullptr) return;

	char* buf = _buf;
	SetChatAllRequest(buf);
}