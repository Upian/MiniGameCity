#ifndef __MANAGEMENT_PACKET_H__
#define __MANAGEMENT_PACKET_H__
#include "BasePacket.h"

// must go to ini file 
#define CHANNEL_SIZE 4
#define ID_SIZE 8
#define PW_SIZE 16
#define NICK_SIZE 24

enum ManagementPacketType : char {
	managementPacketTypeNone = 0,


	managementPacketTypeLoginResponse, //(bool)flag(1), (string)nick(4~8)
	managementPacketTypeLoginRequest, //(string)accountId(4~8), (string)accountPw(8~16)  
	managementPacketTypeLogoutRequest, //nothing
	managementPacketTypeSignupResponse, //(bool)flag
	managementPacketTypeSignupRequest, //(string)accountId(4~8), (string)accountPw(8~16), (string)nick(4~8),
	managementPacketTypeDeleteResponse, //(bool)flag
	managementPacketTypeDeleteRequest, //nothing
	managementPacketTypeShowChannelResponse, //(channel(string, int, int)channelName, numberOfPeople, limitOfPeople)
	managementPacketTypeShowChannelRequest, //nothing.
	managementPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	managementPacketTypeChannelInRequest, //(string)channelName






	managementPacketTypeSize,
};

struct Channel {
	std::string channelName;
	int32 numberOfPeople = 0;
	int32 limitOfPeople = 0;
};




#endif