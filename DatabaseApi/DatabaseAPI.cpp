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

Account DatabaseAPI::LoginAccount(const std::string& userId, const std::string& userPw) {
	Account account{};
	std::string sql = "SELECT * FROM ";
	sql += DB_NAME;
	sql += ".login WHERE ";
	sql += "user_id='" + userId + "' and user_pw='" + userPw + "' and user_status='1'"; // 중복 로그인 작업 예정

	if (mysql_query(conn, sql.c_str())) {
		printf("검색 실패\n");
		account.flag = false;
		return account;
	}

	res = mysql_use_result(conn);
	row = mysql_fetch_row(res);

	if (row != nullptr) {
		id = atoi(row[0]);
		account.flag = true;
		account.nick = row[3];
	}
	else {
		account.flag = false;
		printf("해당 아이디 없음\n");
	}
	mysql_free_result(res);
	sql = "UPDATE ";
	sql += DB_NAME;
	sql += ".login SET date_login=now() WHERE id=" + std::to_string(id);
	if (mysql_query(conn, sql.c_str())) {
		printf("로그인업데이트 실패\n");
		account.flag = false;
		return account;
	}

	return account;
}

void DatabaseAPI::LogoutAccount() {
	std::string sql = "UPDATE ";
	sql += DB_NAME;
	sql += ".login SET date_logout=now() WHERE id=" + std::to_string(id);
	printf("%s\n", sql.c_str());
	if (mysql_query(conn, sql.c_str())) {
		printf("로그아웃다시\n");
	}
}

bool DatabaseAPI::SignUpAccount(const std::string& userId, const std::string& userPw, const std::string& userNick) {
	std::string sql = "INSERT INTO ";
	sql += DB_NAME;
	sql += ".login (user_id, user_pw, user_nick) VALUES ";
	sql += "('" + userId + "', '" + userPw + "', '" + userNick + "')";

	if (mysql_query(conn, sql.c_str())) {
		printf("중복된 계정\n");
		return false;
	}

	mysql_free_result(res);
	sql = "SELECT login.id FROM login WHERE user_id='" + userId + "'";
	if (mysql_query(conn, sql.c_str())) {
		printf("id 가져오기 실패\n");
		return false;
	}
	res = mysql_use_result(conn);
	row = mysql_fetch_row(res);

	id = atoi(row[0]);

	mysql_free_result(res);
	sql = "INSERT INTO ";
	sql += DB_NAME;
	sql += ".user (id) VALUES ";
	sql += "(" + std::to_string(id) + ")";
	if (mysql_query(conn, sql.c_str())) {
		printf("update user 실패\n");
		return false;
	}

	sql = "INSERT INTO ";
	sql += DB_NAME;
	sql += ".ranking (id, ranking) VALUES ";
	sql += "(" + std::to_string(id) + ", " + std::to_string(id) + ")";
	if (mysql_query(conn, sql.c_str())) {
		printf("update ranking 실패\n");
		return false;
	}

	sql = "INSERT INTO ";
	sql += DB_NAME;
	sql += ".inventory (id) VALUES ";
	sql += "(" + std::to_string(id) + ")";

	if (mysql_query(conn, sql.c_str())) {
		printf("update inventory 실패\n");
		return false;
	}

	sql = "INSERT INTO ";
	sql += DB_NAME;
	sql += ".social (id) VALUES ";
	sql += "(" + std::to_string(id) + ")";

	if (mysql_query(conn, sql.c_str())) {
		printf("update social 실패\n");
		return false;
	}
	return true;
}

bool DatabaseAPI::StopAccount() {
	std::string sql = "UPDATE ";
	sql += DB_NAME;
	sql += ".login SET user_status=0 WHERE id=" + std::to_string(id);
	printf("%s\n", sql.c_str());
	if (mysql_query(conn, sql.c_str())) {
		printf("stopAccount 실패\n");
		return false;
	}
	return true;
}