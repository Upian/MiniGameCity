#ifndef __DB_CACHE_H__
#define __DB_CACHE_H__

#include "BaseServer.h"
#include "ClntServerManager.h"
#include "DBCachePacket.h"
#include "SaveData.h"
#include <mysql.h>

class DBCache : public BaseServer<DBCache> {
public:


private:
	MAKE_SERVER(DBCache)
	void HandleSave();
	void HandleLoad();

	bool Connect(const std::string& dbHost, const std::string& dbUser, const std::string& dbPass, const std::string& dbName, const int32& dbPort);
	void Close();
	
	std::list<db::Login> loginData;
	std::list<db::User> userData;
	std::list<db::Friend> friendData;
	std::list<db::Guild> guildData;
	std::list<db::Game> gameData;

	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;
};


#endif