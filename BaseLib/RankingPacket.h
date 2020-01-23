#ifndef RANKINGPACKET_H
#define RANKINGPACKET_H
#include "BasePacket.h"

enum RankingPacketType {
	rankingPacketTypeNone = 0,
	rankingPacketCheckResponse, //(int)ranking
	rankingPacketCheckRequest, //(string)id, 
	rankingPacketUpdateResponse, //(int)rakning
	rankingPacketUpdateRequest, //(int array??)id list,

	rankingPacketTypeSize,
};

#endif