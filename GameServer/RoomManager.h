#pragma once
#ifndef __GAMESERVER_ROOM_MANAGER_H__
#define __GAMESERVER_ROOM_MANAGER_H__
#include <list>
#include <stack>
#include <memory>

#include "Room.h"
#include "RoomPacket.h"

class Player;

class RoomManager{
public:
	~RoomManager();

	void Initialize();

	void HandleRoomPacket(Buffer& buffer, std::shared_ptr<Player> player);
	
	std::shared_ptr<Room> FindRoomByPlayer(std::shared_ptr<Player> pplayer);
	void RemoveRoom(std::shared_ptr<Room> room);

	size_t GetRoomCount() const { return m_roomList.size(); }

	std::list<std::shared_ptr<Room> >& GetRoomList() { return m_roomList; }
	//Packet Handling
	
private:
	void HandlePacketMakeRoom(RoomPacketMakeRoomRequest& packet, std::shared_ptr<Player> master);
	void HandlePacketRoomList(RoomPacketRoomListRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketEnterRoom(RoomPacketEnterRoomRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketLeaveRoom(std::shared_ptr<Player> player);
	void HandlePacketRoomInfo(std::shared_ptr<Player> player);
	void HaldlePacketToggleReady(std::shared_ptr<Player> player);

	void ClearDeactivatedRoom();
	std::thread* m_roomWatcher = nullptr;

	std::list<std::shared_ptr<Room> > m_roomList;
	
	std::stack<int> m_roomNumberList;

	int m_maximumRoomNumber = 100; //#DesignData
	int m_maximumRoomOnePage = 5; //#DesignData
	size_t m_minimumPlayer = 2;	//#DesignData
	size_t m_maximumPlayer = 6;	//#DesignData
};

#endif // !__GAMESERVER_ROOM_MANAGER_H__
