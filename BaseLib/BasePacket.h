#ifndef BASEPACKET_H
#define BASEPACKET_H
#include <iostream>

#define BUFFER_SIZE 1024

enum BasePacketType {
	basePacketTypeNone = 0,
	basePacketTypeLogin,
	basePacketTypeGame,
	basePacketTypeRoom,
	basePacketTypeShop,
	basePacketTypeRanking,
	basePacketTypeSocial,
	basePacketTypeSize,
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
	char data[BUFFER_SIZE] = "";
};

class AllChatPacket : public SocialPacket {
public:
	AllChatPacket() {}
	~AllChatPacket() {}
	virtual void Serialize(AllChatPacket* _packet, char* _buf);
	virtual void Deserialize(char* _buf, AllChatPacket* _packet);
private:
	char data[BUFFER_SIZE] = "";
};

#endif