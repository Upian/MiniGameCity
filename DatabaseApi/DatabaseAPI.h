#ifndef __DB_API_H__
#define __DB_API_H__

#include <winsock2.h>
#include <mysql.h>
#include <string>
#include "Utf8.h"

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "minicity1!"
#define DB_NAME "minigamecity"
#define DB_PORT 3306

typedef int TURN;


struct Account {
	bool flag = true;
	std::string nick;
};

class DatabaseAPI {
public:
	DatabaseAPI(); // connect
	~DatabaseAPI(); // close

	int GetId() {
		return GPID;
	}
	void SetId(unsigned __int32 _GPID) {
		GPID = _GPID;
	}

	bool Connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const int& port);
	void Close();

	Account LoginAccount(const std::string& userId, const std::string& userPw);
	void LogoutAccount();
	bool SignUpAccount(const std::string& userId, const std::string& userPw, const std::string& userNick);
	bool StopAccount();

private:
	MYSQL* conn; // 커낵터
	MYSQL_RES* res; // 결과 값
	MYSQL_ROW row; // 결과 row
	unsigned __int32 GPID  = 0; // id 값
};

#endif // __DB_API_H__