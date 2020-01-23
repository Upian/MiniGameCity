#pragma once
#ifndef __GAMESERVER_LOBBY_H__
#define __GAMESERVER_LOBBY_H__

#include <list>

class Player;

class Room {
public:

private:
	std::list<Player*> m_playerList;

};
#endif // !__GAMESERVER_LOBBY_H__
