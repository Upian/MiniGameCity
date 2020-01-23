#ifndef BASEPACKET_H
#define BASEPACKET_H

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

	//Type -> Byte, (버퍼, 값)
	inline void TypeSerial(char*& _buf, char _type) {
		if (_buf == nullptr) return;

		*_buf = _type;
		++_buf;
	}
	//Byte -> Type, (버퍼)
	inline char TypeDeserial(char*& _buf) {
		if (_buf == nullptr) return 0;

		char type = *_buf;
		++_buf;
		return type;
	}

	//Int -> Byte, (버퍼, 값)
	inline void IntSerial(char*& _buf, int _val) {
		if (_buf == nullptr) return;

		for (int i = 0; i < 4; ++i) {
			*_buf = _val;
			++_buf;
			_val >>= 8;
		}
	}
	//Byte -> Int, (버퍼)
	inline int IntDeserial(char*& _buf) {
		if (_buf == nullptr) return -1;

		int val = 0;
		for (int i = 0; i < 4; ++i) {
			int tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		return val;
	}
	//Bool -> Byte, (버퍼, 불값)
	void BoolSerial(char*& _buf, bool _flag) {
		if (_buf == nullptr) return;

		*_buf = _flag;
		++_buf;
	}
	//Byte -> Bool, (버퍼)
	bool BoolDeserial(char*& _buf) {
		if (_buf == nullptr) return false;

		return *_buf;
	}
	//String -> Byte, (버퍼, 데이터)
	inline void StringSerial(char*& _buf, char* _data) {
		if (_buf == nullptr) return;
		if (_data == nullptr) return;

		while (*_data != '\n') {
			*_buf = *_data;
			++_buf;
			++_data;
		}
		*_buf = '\n';
		++_buf;
	}
	//Byte -> String (버퍼, 데이터)
	inline void StringDeserial(char*& _buf, char* _data) {
		if (_buf == nullptr) return;

		while (*_buf != '\n') {
			*_data = *_buf;
			++_buf;
			++_data;
		}
		*_data = '\n';
		++_buf;
	}

	virtual void Serialize(char* _buf) = 0;
	virtual void Deserialize(char* _buf) = 0;

	BasePacketType GetBasePacketType() {
		return basePacketType;
	}
	void SetBasePacketType(BasePacketType _type) {
		basePacketType = _type;
	}
protected:
	BasePacketType basePacketType = basePacketTypeNone;
};

#endif