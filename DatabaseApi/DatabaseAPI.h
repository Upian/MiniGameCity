//��� ���Ϸ� ������ ��
//�ø� ������ lib�߰��ؼ�

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
	bool flag = true;
	std::string nick;
};

class DatabaseAPI {
public:
	DatabaseAPI(); // connect
	~DatabaseAPI(); // close

	int GetId() {
		return id;
	}
	void SetId(int _id) {
		id = _id;
	}

	bool Connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database, const int& port);
	void Close();

	Account LoginAccount(const std::string& userId, const std::string& userPw);
	void LogoutAccount();
	bool SignUpAccount(const std::string& userId, const std::string& userPw, const std::string& userNick);
	bool StopAccount();

private:
	MYSQL* conn; // Ŀ����
	MYSQL_RES* res; // ��� ��
	MYSQL_ROW row; // ��� row
	int id = 0; // id ��
};



#endif // __DB_API_H__