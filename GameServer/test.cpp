#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

typedef __int32 int32;

//packet type
//세분화하기로 결정
enum Type {
	none = 0,
	login, //id, pw, 
	logout,
	signUp, //id, pw, pwck, email
	chat, //chat(all, whisper, friend, guild)
	ranking, //id
	friendAdd, //id, yes or no
	friendInvite, //id
	guildCreate, //name, src
	guildInvite,
	gameStart,
	room, //title, pw, mode, create, enter, exit
	shopBuy, //id, price
};

//base packet
class BasePacket {
public:
	BasePacket() {}
	~BasePacket() {}
	virtual std::string serialize(Type _type, std::string _data) = 0;
	virtual void deserialize(BasePacket* _basePacket) = 0;
private:
	//header
	Type type = none; //function
	//data
	std::string buf; //data
};

class LoginPacket : public BasePacket {
public:
	LoginPacket() : id("", 8), pw("", 16) {
		sz = id.size() + pw.size();
	}
	~LoginPacket() {}

	std::string serialize(std::string& _id, std::string& _pw) {
		buf =_id + _pw;
		return buf;
	}
	void deserialize(std::string& _packet) {
		id = _packet.substr(8, 8);
		pw = _packet.substr(16, 16);
		/*
		//오류 처리 필요
		if (_packet->sz == 24) {
			id = _packet->buf.substr(0, 8);
			pw = _packet->buf.substr(8, 16);
		}
		*/
	}
	std::string getId() {
		return id;
	}
	std::string getPw() {
		return pw;
	}
public:
	std::string id; //8
	std::string pw; //16
	std::string buf;
	int sz = 0;
};

int main() {
	LoginPacket* loginPacket = 0;
	Type type = login;
	int32 sz = 24;
	std::string id = "abc";
	std::string pw = "123";

	std::string buffer = loginPacket->serialize(type, sz, id, pw);
	std::cout << buffer;

	//loginPacket->deserialize(buffer);
	//cout << loginPacket->getId();

	return 0;
}