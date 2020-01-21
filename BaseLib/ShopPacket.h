#ifndef SHOPPACKET_H
#define SHOPPACKET_H
#include "BasePacket.h"

enum ShopPacketType {
	shopPacketTypeNone = 0,
	shopPacketTypeEnterResponse, //(string 구분자 필요)txt파일에서 목록
	shopPacketTypeEnterRequest, //(string)id
	shopPacketTypeItemBuyResponse, //(bool)yes or no,
	shopPacketTypeItemBuyRequest, //(string)id, (string)item name, (int)item price, (int)owned point, ?? db가 가지고 있어 가지고 있는 돈?
	shopPacketTypeItemSellResponse, //(bool)yes or no
	shopPacketTypeItemSellRequest, //(string)id, (string)item name, (int)owned point, 

	shopPacketTypeSize,
};

#endif