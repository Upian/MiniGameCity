#ifndef BASEPACKET_H
#define BASEPACKET_H

#include <memory>

#define BUFFER_SIZE 1024

typedef __int32 int32;

enum BasePacketType : char {
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

	//int32 -> Byte, (버퍼, 값)
	inline void IntSerial(char*& _buf, int32 _val) {
		if (_buf == nullptr) return;

		for (int32 i = 0; i < sizeof(int32); ++i) {
			*_buf = _val;
			++_buf;
			_val >>= 8;
		}
	}

	//Bool -> Byte, (버퍼, 불값)
	void BoolSerial(char*& _buf, bool _flag) {
		if (_buf == nullptr) return;

		*_buf = _flag;
		++_buf;
	}

	//String -> Byte, (버퍼, 데이터)
	inline void StringSerial(char*& _buf, char* _data) {
		if (_buf == nullptr) return;
		if (_data == nullptr) return;

		int32 len = strlen(_data) + 1;

		memcpy(_buf, _data, len * sizeof(char));
		_buf += len * sizeof(char);
	}

	virtual char* Serialize() = 0;
	virtual void Deserialize(char* _buf) = 0;

	BasePacketType GetBasePacketType() {
		return basePacketType;
	}
	void SetBasePacketType(BasePacketType _type) {
		basePacketType = _type;
	}
private:
	BasePacketType basePacketType = basePacketTypeNone;
};

//Byte -> Type, (버퍼)
inline char TypeDeserial(char*& _buf) {
	if (_buf == nullptr) return 0;

	char type = *_buf;
	++_buf;
	return type;
}

//Byte -> Bool, (버퍼)
inline bool BoolDeserial(char*& _buf) {
	if (_buf == nullptr) return false;

	return *_buf;
}

//Byte -> int32, (버퍼)
inline int32 IntDeserial(char*& _buf) {
	if (_buf == nullptr) return -1;

	int32 val = 0;
	for (int32 i = 0; i < sizeof(int32); ++i) {
		int32 tmp = (*_buf << 8 * i);
		val |= tmp;
		++_buf;
	}
	return val;
}

//Byte -> String (버퍼)
inline char* StringDeserial(char*& _buf) {
	if (_buf == nullptr) return nullptr;

	int32 len = strlen(_buf) + 1;
	char* data = new char[len];

	memcpy(data, _buf, len * sizeof(char));
	_buf += len * sizeof(char);
	return data;
}

#endif