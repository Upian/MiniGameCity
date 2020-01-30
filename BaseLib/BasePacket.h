#ifndef BASEPACKET_H
#define BASEPACKET_H

#include <memory>
#include <string>

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

	//Type -> Byte, (타입)
	inline void TypeSerial(char _type) {
		buf[idx] = _type;
		++idx;
	}

	//Bool -> Byte, (불값)
	inline void BoolSerial(bool _flag) {
		buf[idx] = _flag;
		++idx;
	}

	//Char -> Byte, (캐릭터)
	inline void CharSerial(char _character) {
		buf[idx] = _character;
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
	inline void StringSerial(std::string _data) {
		for (int i = 0; i < _data.size(); ++i) {
			buf[idx] = _data[i];
			++idx;
		}
		buf[idx] = '\n';
		++idx;
	}

	//Byte -> Bool, (버퍼, 불값)
	inline void BoolDeserial(char*& _buf, bool& _flag) {
		if (_buf == nullptr) return;

		bool flag = *_buf;
		_flag = flag;
	}

	//Byte -> Char, (버퍼, 캐릭터)
	inline void CharDeserial(char*& _buf, char& _character) {
		if (_buf == nullptr) return;

		char character = *_buf;
		_character = character;
	}

	//Byte -> int16, (버퍼, 값)
	inline void Int16Deserial(char*& _buf, int16& _val) {
		if (_buf == nullptr) return;

		int16 val = 0;
		for (int32 i = 0; i < sizeof(int16); ++i) {
			int16 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		_val = val;
	}

	//Byte -> int32, (버퍼, 값)
	inline void Int32Deserial(char*& _buf, int32& _val) {
		if (_buf == nullptr) return;

		int32 val = 0;
		for (int32 i = 0; i < sizeof(int32); ++i) {
			int32 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		_val = val;
	}

	//Byte -> int64, (버퍼, 값)
	inline void Int64Deserial(char*& _buf, int64& _val) {
		if (_buf == nullptr) return;

		int64 val = 0;
		for (int32 i = 0; i < sizeof(int64); ++i) {
			int64 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		_val = val;
	}

	//Byte -> String, (버퍼, 데이터)
	inline void StringDeserial(char*& _buf, std::string& _data) {
		if (_buf == nullptr) return;

		std::string data;
		while (*_buf != '\n') {
			data += *_buf;
			++_buf;
		}
		_data = data;
	}

	virtual char* Serialize() = 0;
	virtual void Deserialize(char* _buf) = 0;

	BasePacketType GetBasePacketType() const {
		return basePacketType;
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