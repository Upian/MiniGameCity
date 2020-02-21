#include "LoginDatabaseAPI.h"

DatabaseAPI::DatabaseAPI() {
	
}

DatabaseAPI::~DatabaseAPI() {
	
}

bool DatabaseAPI::Connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const int& port) {
	conn = mysql_init(nullptr);
	mysql_set_character_set(conn, "utf8");
	if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), database.c_str(), port, nullptr, 0)) {
		printf("%s\n", mysql_error(conn)); // ���� ����
		return false;
	}
	return true;
}

void DatabaseAPI::Close() {
	mysql_close(conn);
	printf("%s\n", mysql_error(conn)); // �ݱ� ����
}

Account DatabaseAPI::LoginAccount(const std::string& userId, const std::string& userPw) {
	Account account{};
	try {
		// user_id, user_pw, user_status = 1�̸� �α���.
		std::string sql = "SELECT * FROM ";
		sql += dbName;
		sql += ".login WHERE ";
		sql += "user_id='" + userId + "' and user_pw='" + userPw + "' and user_status='1'";

		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // �˻� ����
			account.flag = false;
			account.nick = "";
			return account;
		}

		res = mysql_use_result(conn);
		row = mysql_fetch_row(res);

		if (row != nullptr) {
			Util::Conversion con;
			gpid = atoi(row[0]);
			account.flag = true;
			account.nick = con.ToAnsi(row[3]);
			if ((atoi(row[6]) - atoi(row[7])) > 0) {
				account.duplicate = true;
			}
			mysql_free_result(res);

			// date_login�� ������Ʈ.
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
			printf("�˻� ��� ����\n");
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
		// user_id, user_status=1�� row�� ������ �ߺ�
		std::string sql = "SELECT * FROM ";
		sql += dbName;
		sql += ".login WHERE ";
		sql += "user_id='" + userId + "' and user_nick='" + userNick + "' and user_status='1'";

		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // �˻� ����
			return false;
		}

		res = mysql_use_result(conn);
		row = mysql_fetch_row(res);

		if (row != nullptr) {
			return false;
		}
		mysql_free_result(res);

		// user_id, user_pw, user_nick�� ����
		Util::Conversion con;
		sql = "INSERT INTO ";
		sql += dbName;
		sql += ".login (user_id, user_pw, user_nick) VALUES ";
		sql += "('" + userId + "', '" + userPw + "', '" + con.ToUTF8(userNick.c_str()) + "')";

		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // �ߺ��� ����
			return false;
		}

		// �ش� gpid�� �˻��Ͽ� �ٸ� ���̺��� �߰��Ѵ�.
		sql = "SELECT login.GPID FROM login WHERE user_id='" + userId + "' and user_status='1'";
		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn)); // gpid�������� ����
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
		// �ȵ��� �� return false�ϴ� �κ��� �̻���..
		if (mysql_query(conn, sql.c_str())) {
			printf("%s\n", mysql_error(conn));

			return false;
		}

		sql = "INSERT INTO ";
		sql += dbName;
		sql += ".ranking (GPID, ranking) VALUES ";
		sql += "(" + std::to_string(gpid) + ", " + std::to_string(gpid) + ")"; // ��ŷ �ϴ� �⺻�� ����
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
		sql += ".social_friend (GPID) VALUES ";
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
			printf("%s\n", mysql_error(conn)); // stop account ����
			return false;
		}
	}
	catch (int e) {

	}
	return true;
}