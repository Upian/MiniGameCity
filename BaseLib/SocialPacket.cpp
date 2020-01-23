#include "SocialPacket.h"

void ChatAllRequest::Serialize(char* _buf) {
	if (_buf == nullptr) return;

	char* buf = _buf;
	TypeSerial(_buf, BasePacket::GetBasePacketType());
	TypeSerial(_buf, SocialPacket::GetSocialPacketType());
	StringSerial(_buf, ChatAllRequest::GetChatAllRequest());
	_buf = buf;
}

void ChatAllRequest::Deserialize(char* _buf) {
	if (_buf == nullptr) return;

	SetBasePacketType((BasePacketType)TypeDeserial(_buf));
	SetSocialPacketType((SocialPacketType)TypeDeserial(_buf));
	StringDeserial(_buf, chatAllRequest);
}

int main() {
	char tmp[50] = { BasePacketType::basePacketTypeSocial, SocialPacketType::socialPacketTypeChatAllRequest, 'h', 'i', '\n' };
	char tmp2[50] = "";

	BasePacket* basePacket = nullptr;
	switch ((BasePacketType)tmp[0]) {
	case BasePacketType::basePacketTypeSocial: {
		switch ((SocialPacketType)tmp[1]) {
		case SocialPacketType::socialPacketTypeChatAllRequest: {
			basePacket = new ChatAllRequest;
			basePacket->Deserialize(tmp);
			delete basePacket;
			basePacket = nullptr;
			break;
		}
		}
		break;
	}
	}

	return 0;
}