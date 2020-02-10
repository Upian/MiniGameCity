#include "DatabaseAPI.h"

DatabaseAPI::DatabaseAPI() {

}

DatabaseAPI::~DatabaseAPI() {


}

bool DatabaseAPI::Connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const int& port) {
	conn = mysql_init(nullptr);
	if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), database.c_str(), port, nullptr, 0)) {
		printf("DB 접속 실패\n");
		return false;
	}
	return true;
}

void DatabaseAPI::Close() {
	mysql_close(conn);
}


Account DatabaseAPI::SearchAccount(const std::string& id, const std::string& password) {
	Account account{};
	std::string sql = "SELECT * FROM ";
	sql += DB_NAME;
	sql += ".login WHERE userId = '";
	sql += id + "' and userPw = '" + password + "'";

	if (mysql_query(conn, sql.c_str())) {
		printf("검색 실패\n");
		account.flag = false;
		return account;
	}
	res = mysql_use_result(conn);
	row = mysql_fetch_row(res);
	if (row != nullptr) {
		account.flag = true;
		account.gold = 100;
		account.identifier = atoi(row[4]);
		account.nick = row[3];
	}
	else {
		account.flag = false;
		printf("해당 아이디 없음\n");
	}
	mysql_free_result(res);
	return account;
}

bool DatabaseAPI::SignUpAccount(const std::string& id, const std::string& password, const std::string& nick) {
	bool result = false;

	std::string sql = "INSERT INTO ";
	sql += DB_NAME;
	sql += ".login (userId, userPw) VALUES";
	sql += " ('" + id + "', '" + password + "')";

	if (mysql_query(conn, sql.c_str())) {
		printf("중복된 계정\n");
		return false;
	}
	result = true;
	return result;
}