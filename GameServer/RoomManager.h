#pragma once
#ifndef __GAMESERVER_ROOM_MANAGER_H__
#define __GAMESERVER_ROOM_MANAGER_H__
#include <list>

#include "Singleton.h"


class Room;
class Player;

class RoomManager : public Util::Singleton<RoomManager> {
public:
	void MakeRoom(size_t maxPlayer, Player* master);
	void DestroyRoom();

//	Room* FindRoom();

	size_t GetRoomCount() const { m_roomList.size(); }

private:
	DECLARE_SINGLETON(RoomManager)
	std::list<Room> m_roomList;
	
	size_t m_minPlayer = 1 ;	//#DesignData
	size_t m_maxPlayer = 6;	//#DesignData
};

#endif // !__GAMESERVER_ROOM_MANAGER_H__
