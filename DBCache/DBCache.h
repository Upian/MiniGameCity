#ifndef __DB_CACHE_H__
#define __DB_CACHE_H__

#include "BaseServer.h"
#include "SaveData.h"
#include "LoadData.h"
#include "ClntServerManager.h"
#include "DBCachePacket.h"

struct Login {
	uint32 gpid = 0;
	std::string userId;
	std::string userPw;
	std::string userStatus;
	// std::string date_created;
	// std::string dateLogin;
	// std::string dateLogout;
};

struct User {
	uint32 gpid = 0;
	int32 gold = 0;
	int32 ranking = 0;
};

struct Friend {
	uint32 gpid = 0;

};

struct Guild {
	uint32 guildId = 0;
	std::string guildName;
	std::string guildMaster;
	std::string guildMember;
	std::string guildLimit;
};

struct Game {
	uint32 gpid;
	int32 point;
};

class DBCache : public BaseServer<DBCache> {
public:


private:
	MAKE_SERVER(DBCache)
	void HandleBasePacket(BufferInfo* bufInfo);
	void HandleSave();
	void HandleLoad();


	std::list<Login> loginData;
	std::list<Friend> friendData;
	std::list<Guild> guildData;
	std::list<Game> gameData;
};


#endif