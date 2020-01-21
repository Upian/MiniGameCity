#pragma once
#ifndef PACKET_H
#define PACKET_H
#include <iostream>

#define BUFSZ 1024
#define RADIX 10

//Response:p
//Request:q
enum Type {
	none = 0,
	login = 10,
	pLogin, //id, pw,  
	qLogin,
	pLogout,
	qLogout,
	pSignUp, //id, pw, pwck, email 
	qSignUp,
	chat = 20,
	pChat,
	allChat,
	whisperChat,
	guildChat,
	friendAdd, //id, yes or no 
	friendInvite, //id 
	guildCreate, //name, src 
	guildInvite,
	gameStart,
	room, //title, pw, mode, create, enter, exit 
	shopBuy, //id, price 
	ranking = 60 //id 
};

class BasePacket {
public:
	BasePacket() {}
	virtual ~BasePacket() {}
	//�޸� ����
	//���� ó��
	//����� ���ڱ� ����ٸ� 4����Ʈ ������ �� �ִ�.
	//�߰��� �����ڷ� �Ұ����� ������ ���� ������
	//Int -> Byte, (��, ����, ���۽�����ġ + ũ��)
	inline void IntSerial(int _val, char* _buf, int _st) {
		if (_buf != nullptr && (_buf + _st != nullptr)) {
			_buf = _buf + _st - 1;
			//szȮ������ ����
			while (_val) {
				*_buf-- = (_val % 10) + '0';
				_val /= 10;
			}
			_buf = _buf - _st;
		}
	}
	//String -> Byte, (��, ����, ���۽�����ġ)
	inline void StringSerial(char* _data, char* _buf, int _st) {
		if (_data != nullptr && _buf != nullptr) {
			_buf = _buf + _st;
			while (*_data != '\n') {
				*_buf++ = *_data++;
			}
			_buf = _buf - _st - 1;
		}
	}
	//BasePacket -> Buffer
	virtual void Serialize(BasePacket* _packet, char* _buf);
	//Byte -> Int, (����, ��, ũ��)
	inline void IntDeserial(char*& _buf, int& _val, int len) {
		if (_buf != nullptr) {
			char tmp[10] = ""; //sz ����
			int idx = 0;
			while (*_buf <= '9' && *_buf >= '0' && idx < len) {
				tmp[idx++] = *_buf++;
			}
			_val = std::atoi(tmp);
		}
	}
	//Byte -> String (����, ��, ũ��)
	inline void StringDeserial(char* _buf, char* _data, int len) {
		if (_buf != nullptr) {
			int idx = 0;
			while (*_buf != '\n' && idx < len) {
				*_data++ = *_buf++;
			}
		}
	}
	//buffer -> BasePacket
	virtual void Deserialize(char* _buf, BasePacket* _packet);
	int GetType() {
		return type;
	}
	void SetType(int _type) {
		type = _type;
	}
	char* GetData() {
		return data;
	}
	void SetData(char* _data) {
		*data = *_data;
	}
private:
	int type = 0;
	char data[BUFSZ] = "";
};

class BaseChatPacket : public BasePacket {
public:
	BaseChatPacket() {}
	~BaseChatPacket() {}
	virtual void Serialize(BaseChatPacket* _packet, char* _buf);
	virtual void Deserialize(char* _buf, BaseChatPacket* _packet);
private:
	int chatType = 0;
	char data[BUFSZ] = "";
};

class AllChatPacket : public BaseChatPacket {
public:
	AllChatPacket() {}
	~AllChatPacket() {}
	virtual void Serialize(AllChatPacket* _packet, char* _buf);
	virtual void Deserialize(char* _buf, AllChatPacket* _packet);
private:
	char data[BUFSZ] = "";
};

#endif