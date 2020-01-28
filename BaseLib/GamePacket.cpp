#include "GamePacket.h"
#include <iostream>

struct simpleTypePacket
{
	int game_mode;
	int packet_type;
};
struct intTypePacket
{
	int game_mode;
	int packet_type;
	int int_data;
};
struct charTypePacket
{
	int game_mode;
	int packet_type;
	char char_array[];
};