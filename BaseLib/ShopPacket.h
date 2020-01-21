#ifndef SHOPPACKET_H
#define SHOPPACKET_H
#include "BasePacket.h"

enum ShopPacketType {
	shopPacketTypeNone = 0,
	shopPacketTypeEnterResponse, //(string ������ �ʿ�)txt���Ͽ��� ���
	shopPacketTypeEnterRequest, //(string)id
	shopPacketTypeItemBuyResponse, //(bool)yes or no,
	shopPacketTypeItemBuyRequest, //(string)id, (string)item name, (int)item price, (int)owned point, ?? db�� ������ �־� ������ �ִ� ��?
	shopPacketTypeItemSellResponse, //(bool)yes or no
	shopPacketTypeItemSellRequest, //(string)id, (string)item name, (int)owned point, 

	shopPacketTypeSize,
};

#endif