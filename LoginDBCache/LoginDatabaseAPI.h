#ifndef __LOGIN_DB_API_H__
#define __LOGIN_DB_API_H__

#include <winsock2.h>
#include <mysql.h>
#include <string>
#include "Utf8.h"
#include "Config.h"

struct Account {
	bool flag = true;
	std::string nick;
};

class DatabaseAPI {
public:
	DatabaseAPI();
	~DatabaseAPI(); 

	int GetGpid() {
		return gpid;
	}
	void SetGpid(unsigned __int32 _gpid) {
		gpid = _gpid;
	}

	bool Connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const int& port);
	void Close();

	Account LoginAccount(const std::string& userId, const std::string& userPw);
	void LogoutAccount();
	bool SignUpAccount(const std::string& userId, const std::string& userPw, const std::string& userNick);
	bool StopAccount();

private:
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;
	unsigned __int32 gpid  = 0;

	std::string dbHost = Util::GetConfigToString("LoginDBCache.ini", "DB", "Host", "127.0.0.1");
	std::string dbUser = Util::GetConfigToString("LoginDBCache.ini", "DB", "User", "root");
	std::string dbPass = Util::GetConfigToString("LoginDBCache.ini", "DB", "Pass", "minicity1!");
	std::string dbName = Util::GetConfigToString("LoginDBCache.ini", "DB", "Name", "minigamecity");
	int dbPort = Util::GetConfigToInt("LoginDBCache.ini", "DB", "Port", 3306);
};

#endif // __DB_API_H__