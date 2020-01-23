#ifndef LOBBYPACKET_H
#define LOBBYPACKET_H
#include "BasePacket.h"

enum LobbyPacketType {
	lobbyPacketTypeNone = 0,
	lobbyPacketTypeCreateResponse, //(int)room number
	lobbyPacketTypeCreateRequest, //(string)id, (string)roomname, (bool)roompw ����, (string)roompw, (int??)mode, (int)�ִ� �ο�
	lobbyPacketTypeEnterResponse, //(bool)yes or no, (int)�ο� ��, (string)�ο� id�� (string)nick,
	lobbyPacketTypeEnterRequest, //(int)room number,
	lobbyPacketTypeExitResponse, //(string)id, (string ������ �ʿ�)room list
	lobbyPacketTypeExitRequest, //(string)id, (int)room number
	//room ���� ���� ����
	lobbyPacketTypeSize,
};

#endif