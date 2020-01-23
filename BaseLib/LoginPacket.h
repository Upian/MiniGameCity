#ifndef LOGINPACKET_H
#define LOGINPACKET_H
#include "BasePacket.h"

enum LoginPacketType {
	loginPacketTypeNone = 0,
	loginPacketTypeLoginResponse, //(bool)yes or no, ������ ��� �Ͱ� id(8byte?) ����, (string ������)channel list, (int)�ο� ��,
	loginPacketTypeLoginRequest, //(string)����id, (string)pw 
	loginPacketTypeLogoutResponse, //(bool)yes or no
	loginPacketTypeLogoutRequest, //(bool)id
	loginPacketTypeSignupResponse, //(bool)yes or no
	loginPacketTypeSignupRequest, //(string)����id, (string)pw, (string)nick,
	loginPacketTypeDeleteResponse, //(bool)yes or no
	loginPacketTypeDeleteRequest, //(string)����id, (string)pw
	loginPacketTypeChannelInResonse, //(bool)yes or no, (string)ip, (int)port
	loginPacketTypeChannelInRequest, //(string)id, (int)channel num
	loginPacketTypeChannelOutResponse, //(string)id, (bool)yes or no
	loginPacketTypeChannelOutRequest, //(string)id
	loginPacketTypeSize,
};

#endif