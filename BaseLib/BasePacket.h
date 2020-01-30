#ifndef BASEPACKET_H
#define BASEPACKET_H

#include <memory>

#define BUFFER_SIZE 1024

typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

/*
	=== EXAMPLE ===
	대분류 -> BasePacketType basePacketType = (BasePacketType)TypeDeserial((char*&)buf);
	중분류 -> SocialPacketType socialPacketType = (SocialPacketType)TypeDeserial((char*&)buf);

	=== Deserialize ===
	ChatAllRequest chat{};
	chat.Deserialize(buf);

	=== Serialize ===
	char* buf = chat.Serialize();
*/

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
	BasePacket(BasePacketType _basePacketType) : basePacketType(_basePacketType) {}
	virtual ~BasePacket() {}

	virtual char* Serialize() = 0;
	virtual void Deserialize(char* _buf) = 0;

	BasePacketType GetBasePacketType() const {
		return basePacketType;
	}
protected:
	//Type -> Byte, (값)
	inline void TypeSerial(char _type) {
		buf[idx] = _type;
		++idx;
	}

	//Bool -> Byte, (불값)
	void BoolSerial(bool _flag) {
		buf[idx] = _flag;
		++idx;
	}

	//Char -> Byte, (값)
	inline void CharSerial(char _type) {
		buf[idx] = _type;
		++idx;
	}

	//int16 -> Byte, (값)
	inline void Int16Serial(int16 _val) {
		for (int32 i = 0; i < sizeof(int16); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}

	//int32 -> Byte, (값)
	inline void Int32Serial(int32 _val) {
		for (int32 i = 0; i < sizeof(int32); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}

	//int64 -> Byte, (값)
	inline void Int64Serial(int64 _val) {
		for (int32 i = 0; i < sizeof(int64); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}

	//String -> Byte, (데이터)
	inline void StringSerial(const char* _data) {
		if (_data == nullptr) return;

		while (*_data != NULL) {
			buf[idx] = *_data;
			++idx;
			++_data;
		}
		buf[idx] = '\n';
		++idx;
	}

	//Byte -> Bool, (버퍼)
	inline bool BoolDeserial(char*& _buf) {
		if (_buf == nullptr) return false;

		return *_buf;
	}

	//Byte -> Char, (버퍼)
	inline bool CharDeserial(char*& _buf) {
		if (_buf == nullptr) return false;

		return *_buf;
	}

	//Byte -> int16, (버퍼)
	inline int16 Int16Deserial(char*& _buf) {
		if (_buf == nullptr) return -1;

		int16 val = 0;
		for (int32 i = 0; i < sizeof(int16); ++i) {
			int16 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		return val;
	}

	//Byte -> int32, (버퍼)
	inline int32 Int32Deserial(char*& _buf) {
		if (_buf == nullptr) return -1;

		int32 val = 0;
		for (int32 i = 0; i < sizeof(int32); ++i) {
			int32 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		return val;
	}

	//Byte -> int64, (버퍼)
	inline int64 Int64Deserial(char*& _buf) {
		if (_buf == nullptr) return -1;

		int64 val = 0;
		for (int32 i = 0; i < sizeof(int64); ++i) {
			int64 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		return val;
	}

	//Byte -> String, (버퍼)
	inline char* StringDeserial(char*& _buf) {
		if (_buf == nullptr) return nullptr;

		int len = 0;
		while (*_buf != '\n') {
			++_buf;
			++len;
		}
		_buf -= len;
		++len;

		char* data = new char[len];
		memcpy(data, _buf, len * sizeof(char) - 1);
		data[len - 1] = NULL;

		_buf += len * sizeof(char);
		return data;
	}

	
	void SetBasePacketType(BasePacketType _type) {
		basePacketType = _type;
	}
protected:
	char buf[BUFFER_SIZE]{};
	int32 idx = 0;
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

#endif