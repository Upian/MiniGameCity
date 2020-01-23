#ifndef LOGINPACKET_H
#define LOGINPACKET_H
#include "BasePacket.h"

enum LoginPacketType {
	loginPacketTypeNone = 0,
	loginPacketTypeLoginResponse, //(bool)yes or no, 유저의 모든 것과 id(8byte?) 전달, (string 구분자)channel list, (int)인원 수,
	loginPacketTypeLoginRequest, //(string)실제id, (string)pw 
	loginPacketTypeLogoutResponse, //(bool)yes or no
	loginPacketTypeLogoutRequest, //(bool)id
	loginPacketTypeSignupResponse, //(bool)yes or no
	loginPacketTypeSignupRequest, //(string)실제id, (string)pw, (string)nick,
	loginPacketTypeDeleteResponse, //(bool)yes or no
	loginPacketTypeDeleteRequest, //(string)실제id, (string)pw
	loginPacketTypeChannelInResonse, //(bool)yes or no, (string)ip, (int)port
	loginPacketTypeChannelInRequest, //(string)id, (int)channel num
	loginPacketTypeChannelOutResponse, //(string)id, (bool)yes or no
	loginPacketTypeChannelOutRequest, //(string)id
	loginPacketTypeSize,
};

#endif