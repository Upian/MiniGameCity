#pragma once
#ifndef __GAMESERVER_ROOM_MANAGER_H__
#define __GAMESERVER_ROOM_MANAGER_H__
#include <list>

#include "Singleton.h"
#include "Room.h"

class RoomManager : public Util::Singleton<RoomManager> {
public:
	void MakeRoom();
	void DestroyRoom();


private:
	DECLARE_SINGLETON(RoomManager)
//	std::list<Room> m_rooms;

};

#endif // !__GAMESERVER_ROOM_MANAGER_H__
