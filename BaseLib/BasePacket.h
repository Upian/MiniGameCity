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
	��з� -> BasePacketType basePacketType = (BasePacketType)TypeDeserial((char*&)buf);
	�ߺз� -> SocialPacketType socialPacketType = (SocialPacketType)TypeDeserial((char*&)buf);

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
	BasePacket(BasePacketType _basePacketType) : basePacketType(_basePacketType) {
		this->TypeSerial(basePacketType);
	}
	virtual ~BasePacket() {}

protected:
#pragma region Serialize

	//String -> Byte, (������)
	inline void StringSerial(std::string _data) {
		for (int i = 0; i < _data.size(); ++i) {
			buf[idx] = _data[i];
			++idx;
		}
		buf[idx] = '\n';
		++idx;
	}

	//Char -> Byte, (ĳ����)
	inline void TypeSerial(char _character) {
		buf[idx] = _character;
		++idx;
	}
	//int16 -> Byte, (��)
	inline void TypeSerial(int16 _val) {
		for (int32 i = 0; i < sizeof(int16); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}
	//int32 -> Byte, (��)
	inline void TypeSerial(int32 _val) {
		for (int32 i = 0; i < sizeof(int32); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}
	//int64 -> Byte, (��)
	inline void TypeSerial(int64 _val) {
		for (int32 i = 0; i < sizeof(int64); ++i) {
			buf[idx] = _val;
			++idx;
			_val >>= 8;
		}
	}
#pragma endregion
	//Byte -> String, (����, ������)
	inline void StringDeserial(char*& _buf, std::string& _data) {
		if (_buf == nullptr) return;

		std::string data;
		while (*_buf != '\n') {
			data += *_buf;
			++_buf;
		}
		++_buf;
		_data = data;
	}
	//*
	template<typename T_Arg>
	inline void TypeDeserial(char*& _buf, T_Arg& _val) {
		if (nullptr == _buf)
			return;

		T_Arg val = 0;
		for (int i = 0; i < sizeof(T_Arg); ++i) {
			T_Arg tmp = (*_buf << 8 * 1);
			val |= tmp;
			++_buf;
		}
		_val = val;
	}
	/*/
	

	//Byte -> Bool, (����, �Ұ�)
	inline void BoolDeserial(char*& _buf, bool& _flag) {
		if (_buf == nullptr) return;

		bool flag = *_buf;
		_flag = flag;
	}

	//Byte -> Char, (����, ĳ����)
	inline void TypeDeserial(char*& _buf, char& _character) {
		if (_buf == nullptr) return;

		char character = *_buf;
		_character = character;
	}

	//Byte -> int16, (����, ��)
	inline void TypeDeserial(char*& _buf, int16& _val) {
		if (_buf == nullptr) return;

		int16 val = 0;
		for (int32 i = 0; i < sizeof(int16); ++i) {
			int16 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		_val = val;
	}

	//Byte -> int32, (����, ��)
	inline void TypeDeserial(char*& _buf, int32& _val) {
		if (_buf == nullptr) return;

		int32 val = 0;
		for (int32 i = 0; i < sizeof(int32); ++i) {
			int32 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		_val = val;
	}

	//Byte -> int64, (����, ��)
	inline void TypeDeserial(char*& _buf, int64& _val) {
		if (_buf == nullptr) return;

		int64 val = 0;
		for (int32 i = 0; i < sizeof(int64); ++i) {
			int64 tmp = (*_buf << 8 * i);
			val |= tmp;
			++_buf;
		}
		_val = val;
	}
	//*/
	

	char buf[BUFFER_SIZE]{};
	int32 idx = 0;

public:
	virtual char* Serialize() = 0;
	virtual void Deserialize(char* _buf) = 0;

	BasePacketType GetBasePacketType() const {
		return basePacketType;
	}
	void SetBasePacketType(BasePacketType _type) {
		basePacketType = _type;
	}

private:
	BasePacketType basePacketType = basePacketTypeNone;
};

//Byte -> Type, (����)
inline char TypeDeserial(char*& _buf) {
	if (_buf == nullptr) return 0;

	char type = *_buf;
	++_buf;
	return type;
}

#endif