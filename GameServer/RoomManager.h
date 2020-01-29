#pragma once
#ifndef __GAMESERVER_ROOM_MANAGER_H__
#define __GAMESERVER_ROOM_MANAGER_H__
#include <list>
#include <stack>

#include "Room.h"
#include "RoomPacket.h"

class Player;

class RoomManager{
public:
	~RoomManager();

	void MakeRoom(size_t maxPlayer, Player* master);

	void Initialize();
	Room& FindRoom(Player* pplayer);

	size_t GetRoomCount() const { m_roomList.size(); }

	//Packet Handling
	
private:
	void ClearDeactivatedRoom();
	std::thread* m_roomWatcher = nullptr;

	std::list<Room> m_roomList;
	
	std::stack<int> m_roomNumberList;
	int m_maxRoomNumber = 100; //#DesignData

	size_t m_minPlayer = 1;	//#DesignData
	size_t m_maxPlayer = 6;	//#DesignData
};

#endif // !__GAMESERVER_ROOM_MANAGER_H__
