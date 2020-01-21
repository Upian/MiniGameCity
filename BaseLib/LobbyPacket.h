#ifndef LOBBYPACKET_H
#define LOBBYPACKET_H
#include "BasePacket.h"

enum LobbyPacketType {
	lobbyPacketTypeNone = 0,
	lobbyPacketTypeCreateResponse, //(int)room number
	lobbyPacketTypeCreateRequest, //(string)id, (string)roomname, (bool)roompw 여부, (string)roompw, (int??)mode, (int)최대 인원
	lobbyPacketTypeEnterResponse, //(bool)yes or no, (int)인원 수, (string)인원 id와 (string)nick,
	lobbyPacketTypeEnterRequest, //(int)room number,
	lobbyPacketTypeExitResponse, //(string)id, (string 구분자 필요)room list
	lobbyPacketTypeExitRequest, //(string)id, (int)room number
	//room 세팅 변경 미정
	lobbyPacketTypeSize,
};

#endif