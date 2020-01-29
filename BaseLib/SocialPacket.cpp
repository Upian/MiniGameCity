#include "SocialPacket.h"

char* ChatAllRequest::Serialize() {
	TypeSerial(GetBasePacketType());
	TypeSerial(GetSocialPacketType());
	StringSerial(GetChatAllRequest());
	return buf;
}

void ChatAllRequest::Deserialize(char* _buf) {
	if (_buf == nullptr) return;

	char* buf = _buf;
	SetChatAllRequest(StringDeserial(buf));
}