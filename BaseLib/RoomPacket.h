#ifndef ROOMPACKET_H
#define ROOMPACKET_H
#include "BasePacket.h"

enum RoomPacketType {
	roomPacketTypeNone = 0,
	roomPacketTypeStartResponse, //(bool)yes or no
	roomPacketTypeStartRequest, //(int)room number, 
	roomPacketTypeChatResponse, //(string)id, (string)data
	roomPacketTypeChatRequest, //(string)id, (string)data
	roomPacketTypeReadyResonse, //(string 구분자 필요)방에 있는 인원들
	roomPacketTypeReadyRequest, //(string)id, 
	//ready 하는 건가요??
	roomPacketTypeReadyCancelResponse, //(string 구분자 필요)방에 있는 인원들
	roomPacketTypeReadyCancelResponse, //(string)id,

	roomPacketTypeSize,
};

#endif