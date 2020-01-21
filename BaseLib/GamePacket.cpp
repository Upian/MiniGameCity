#include "GamePacket.h"

struct startMaster {
	int type;
	int room_number;
	int game_mode;
	int player_count;
	char playerID[10];
};
struct startUser 
{
	int type;
	int room_number;
	char playerID[10];
};
struct startPlayerLocation 
{
	int type;
	int color;
	char a[10] = { NULL, }, b[10] = { NULL, }, c[10] = { NULL, }, d[10] = { NULL, }, e[10] = { NULL, };
};
struct startPlayerReady 
{
	int type;
};
struct startServerTimerStart 
{
	int type;
};
struct startServerTimerEnd
{
	int type;
};