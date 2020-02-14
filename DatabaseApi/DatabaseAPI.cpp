#include "DatabaseAPI.h"

DatabaseAPI::DatabaseAPI() {
	
}

DatabaseAPI::~DatabaseAPI() {
	
}

bool DatabaseAPI::Connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const int& port) {
	conn = mysql_init(nullptr);
	mysql_set_character_set(conn, "utf8");
	if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), database.c_str(), port, nullptr, 0)) {
		printf("%s\n", mysql_error(conn)); // 접속 오류
		return false;
	}
	return true;
}

void DatabaseAPI::Close() {
	mysql_close(conn);
	printf("%s\n", mysql_error(conn)); // 닫기 오류
}

Account DatabaseAPI::LoginAccount(const std::string& userId, const std::string& userPw) {
	Account account{};
	try {
		std::string sql = "SELECT * FROM ";
		sql += dbName;
		sql += ".login WHERE ";
		sql += "user_id='" + userId + "' and user_pw='" + userPw + "' and user_status='1'";

		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // 검색 실패
			account.flag = false;
			account.nick = "";
			return account;
		}

		res = mysql_use_result(conn);
		row = mysql_fetch_row(res);

		if (row != nullptr) {
			//// 중복 로그인 작업 예정

			Util::Conversion con;
			gpid = atoi(row[0]);
			account.flag = true;
			account.nick = con.ToAnsi(row[3]);
			mysql_free_result(res);

			sql = "UPDATE ";
			sql += dbName;
			sql += ".login SET date_login=now() WHERE GPID=" + std::to_string(gpid);
			if (mysql_query(conn, sql.c_str())) {
				printf("%s\n", mysql_error(conn));
				account.flag = false;
				account.nick = "";
				return account;
			}
		}
		else {
			account.flag = false;
			account.nick = "";
			printf("검색 결과 없음\n");
		}
	}
	catch (int e) {

	}
	return account;
}

void DatabaseAPI::LogoutAccount() {
	try {
		std::string sql = "UPDATE ";
		sql += dbName;
		sql += ".login SET date_logout=now() WHERE GPID=" + std::to_string(gpid);
		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn));
		}
	}
	catch (int e) {

	}
}

bool DatabaseAPI::SignUpAccount(const std::string& userId, const std::string& userPw, const std::string& userNick) {
	try {
		Util::Conversion con;
		std::string sql = "INSERT INTO ";
		sql += dbName;
		sql += ".login (user_id, user_pw, user_nick) VALUES ";
		sql += "('" + userId + "', '" + userPw + "', '" + con.ToUTF8(userNick.c_str()) + "')";

		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // 중복된 계정
			return false;
		}
		mysql_free_result(res);

		sql = "SELECT login.GPID FROM login WHERE user_id='" + userId + "'";
		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // gpid가져오기 실패
			return false;
		}
		res = mysql_use_result(conn);
		row = mysql_fetch_row(res);
		gpid = atoi(row[0]);
		mysql_free_result(res);

		sql = "INSERT INTO ";
		sql += dbName;
		sql += ".user (GPID) VALUES ";
		sql += "(" + std::to_string(gpid) + ")";
		// 안됐을 때 return false하는 부분이 이상함..
		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn));

			return false;
		}

		sql = "INSERT INTO ";
		sql += dbName;
		sql += ".ranking (GPID, ranking) VALUES ";
		sql += "(" + std::to_string(gpid) + ", " + std::to_string(gpid) + ")"; // 랭킹 일단 기본값 넣음
		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn));
			return false;
		}

		sql = "INSERT INTO ";
		sql += dbName;
		sql += ".inventory (GPID) VALUES ";
		sql += "(" + std::to_string(gpid) + ")";

		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn));
			return false;
		}

		sql = "INSERT INTO ";
		sql += dbName;
		sql += ".social (GPID) VALUES ";
		sql += "(" + std::to_string(gpid) + ")";

		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn));
			return false;
		}
	}
	catch (int e) {

	}
	
	return true;
}

bool DatabaseAPI::StopAccount() {
	try {
		std::string sql = "UPDATE ";
		sql += dbName;
		sql += ".login SET user_status=0 WHERE GPID=" + std::to_string(gpid);
		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // stop account 실패
			return false;
		}
	}
	catch (int e) {

	}
	return true;
}