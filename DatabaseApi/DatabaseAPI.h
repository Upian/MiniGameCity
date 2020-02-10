//헤더 파일로 만들어야 함
//올릴 때에는 lib추가해서

#ifndef __DB_API_H__
#define __DB_API_H__

#include <winsock2.h>
#include <mysql.h>
#include <string>

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "minicity1!"
#define DB_NAME "minigamecity"
#define DB_PORT 3306

typedef int TURN;


struct Account {
	bool flag = false;
	int identifier = 0;
	int gold = 0;
	std::string nick = "";
};

class DatabaseAPI {
public:
	DatabaseAPI(); // connect
	~DatabaseAPI(); // close

	bool Connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const int& port);
	void Close();

	Account SearchAccount(const std::string& id, const std::string& password);
	bool SignUpAccount(const std::string& id, const std::string& password, const std::string& nick);

	//packet GetFromDB(TURN turn) {

	//}

private:
	MYSQL* conn; // 커낵터
	MYSQL_RES* res; // 결과 값
	MYSQL_ROW row; // 결과 row
};



#endif // __DB_API_H__