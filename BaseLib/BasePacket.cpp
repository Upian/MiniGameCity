#include <iostream>

#include "BasePacket.h"
//#include "Log.h"

using namespace std;

void BasePacket::Serialize(BasePacket* _packet, char* _buf) {
	if (_packet == nullptr) return;
	if (_buf == nullptr) return;

	cout << "Serialize ==> " << "buf:" << _buf << '\n';

}

void BasePacket::Deserialize(char* _buf, BasePacket* _packet) {
	if (_buf == nullptr) return;
	if (_packet == nullptr) return;

	IntDeserial((char*&)_buf, _packet->type, 2);
	StringDeserial((char*)_buf, _packet->data, BUFFER_SIZE);
	cout << "Deserialize ==> " << "type:" << type << ' ' << "data:" << data << '\n';
	switch (type) {
		case basePacketTypeSocial: {
			SocialPacket* packet = new SocialPacket;
			packet->Deserialize(_buf, packet);
			delete packet;
			packet = nullptr;
		}
	}
}

void BaseChatPacket::Serialize(BaseChatPacket* _packet, char* _buf) {
	if (_packet == nullptr) return;
	if (_buf == nullptr) return;

	IntSerial(chat, (char*)_buf, 2);
	cout << "Serialize ==> " << "buf:" << _buf << '\n';
	BasePacket* packet = new BasePacket;
	packet->Serialize(packet, _buf);
	delete packet;
	packet = nullptr;
}

void BaseChatPacket::Deserialize(char* _buf, BaseChatPacket* _packet) {
	if (_buf == nullptr) return;
	if (_packet == nullptr) return;

	IntDeserial((char*&)_buf, _packet->chatType, 2);
	StringDeserial((char*)_buf, _packet->data, BUFSZ);
	cout << "Deserialize ==> " << "chatType:" << chatType << ' ' << "data:" << data << '\n';
	switch (chatType) {
		case allChat: {
			AllChatPacket* packet = new AllChatPacket;
			packet->Deserialize(_buf, packet);
			delete packet;
			packet = nullptr;
		}
	}
}

void AllChatPacket::Serialize(AllChatPacket* _packet, char* _buf) {
	if (_packet == nullptr) return;
	if (_buf == nullptr) return;

	IntSerial(allChat, (char*)_buf, 4);
	StringSerial(data, (char*)_buf, 4);
	cout << "Serialize ==> " << "buf:" << _buf << '\n';
	BaseChatPacket* packet = new BaseChatPacket;
	packet->Serialize(packet, _buf);
	delete packet;
	packet = nullptr;
}

void AllChatPacket::Deserialize(char* _buf, AllChatPacket* _packet) {
	if (_buf == nullptr) return;
	if (_packet == nullptr) return;

	StringDeserial((char*)_buf, _packet->data, BUFSZ);
	cout << "Deserialize ==> " << "data:" << data << '\n';
}

int main() {
	char buf2[BUFSZ] = "2022abc@3fj i8918\n";
	BasePacket* packet2 = new BasePacket;
	packet2->Deserialize(buf2, packet2);
	delete packet2;
	packet2 = nullptr;

	char buf[BUFSZ] = "";
	AllChatPacket* packet = (AllChatPacket*)packet2;
	packet->Serialize(packet, buf);
	delete packet;
	packet = nullptr;

	return 0;
}