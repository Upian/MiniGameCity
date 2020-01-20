#pragma once
#ifndef __BASELIB_PLAYER_MANAGER_H__
#define __BASELIB_PLAYER_MANAGER_H__

#include "ServerCommon.h"
#include "Singleton.h"
#include <list>

class Player;


class PlayerManager : public Util::Singleton<PlayerManager>{
public:
	void PlayerLogin();
	void PlayerLogout();
	void PlayerTransferServer();


private:
	DECLARE_SINGLETON(PlayerManager)
	std::list<Player*> m_playerList;

};


#endif // !__BASELIB_PLAYER_MANAGER_H__

