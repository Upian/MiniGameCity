#ifndef BASEPACKET_H
#define BASEPACKET_H

#include <memory>
#include <string>
#include <type_traits>

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

namespace {
	int32 idx = 0;
}

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
	BasePacket(BasePacketType _basePacketType) : basePacketType(_basePacketType) {
		this->PacketTypeSerial(basePacketType);
	}
	virtual ~BasePacket() {}

	virtual char* Serialize() = 0;
	virtual void Deserialize(char* _buf) = 0;

	BasePacketType GetBasePacketType() const {
		return basePacketType;
	}
	void SetBasePacketType(BasePacketType _type) {
		basePacketType = _type;
	}

protected:
#pragma region Serialize

	//Type -> Byte, (타입)
	inline void PacketTypeSerial(char _type) {
		buf[idx] = _type;
		++idx;
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

	//Value -> Byte, (값)
	inline void TypeSerial(char _val) {
		for (int i = 0; i < sizeof(char); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}

	inline void TypeSerial(bool _val) {
		for (int i = 0; i < sizeof(bool); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}

	inline void TypeSerial(int16 _val) {
		for (int i = 0; i < sizeof(int16); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}

	inline void TypeSerial(int32 _val) {
		for (int i = 0; i < sizeof(int32); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}

	inline void TypeSerial(int64 _val) {
		for (int i = 0; i < sizeof(int64); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}
#pragma endregion

	//Byte -> String, (버퍼, 데이터)
	inline void StringDeserial(char* _buf, std::string& _data) {
		if (_buf == nullptr) return;

		std::string data;
		while (*_buf != '\n') {
			data += _buf[idx];
			++_buf;
		}
		++_buf;
		_data = data;
	}

	//Byte -> Value, (버퍼, 값)
	template<typename T_Arg>
	inline void TypeDeserial(char* _buf, T_Arg& _val) {
		if (nullptr == _buf)
			return;

		T_Arg val = 0;
		for (int i = 0; i < sizeof(T_Arg); ++i) {
			T_Arg tmp = (_buf[idx] << 8 * 1);
			val |= tmp;
			++idx;
		}
		_val = val;
	}

protected:
	char buf[BUFFER_SIZE]{};
private:
	BasePacketType basePacketType = basePacketTypeNone;
};

//Byte -> Type, 
inline char PacketTypeDeserial(char* _buf) {
	if (nullptr == _buf) return NULL;

	char type = _buf[idx];
	++idx;
	return type;
}

#endif