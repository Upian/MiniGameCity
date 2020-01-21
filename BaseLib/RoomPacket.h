#ifndef ROOMPACKET_H
#define ROOMPACKET_H
#include "BasePacket.h"

enum RoomPacketType {
	roomPacketTypeNone = 0,
	roomPacketTypeStartResponse, //(bool)yes or no
	roomPacketTypeStartRequest, //(int)room number, 
	roomPacketTypeChatResponse, //(string)id, (string)data
	roomPacketTypeChatRequest, //(string)id, (string)data
	roomPacketTypeReadyResonse, //(string ������ �ʿ�)�濡 �ִ� �ο���
	roomPacketTypeReadyRequest, //(string)id, 
	//ready �ϴ� �ǰ���??
	roomPacketTypeReadyCancelResponse, //(string ������ �ʿ�)�濡 �ִ� �ο���
	roomPacketTypeReadyCancelResponse, //(string)id,

	roomPacketTypeSize,
};

#endif