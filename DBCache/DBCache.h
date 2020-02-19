#ifndef __DB_CACHE_H__
#define __DB_CACHE_H__

#include "BaseServer.h"
#include "ClntServerManager.h"
#include "DBCachePacket.h"
#include <mysql.h>


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
	uint32 friendGpid = 0;
	std::string friendName;
	std::string modifyDate;
};

struct Friends {
	uint32 gpid = 0;
	std::list<Friend> friends;
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
	void HandleSave();
	void HandleLoad();

	bool Connect(const std::string& dbHost, const std::string& dbUser, const std::string& dbPass, const std::string& dbName, const int32& dbPort);
	void Close();

	std::list<Login> loginData;
	std::list<User> userData;
	std::list<Friend> friendData;
	std::list<Guild> guildData;
	std::list<Game> gameData;

	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;
};


#endif