#pragma once
#ifndef __BASELIB_SERVER_COMMON_H__
#define __BASELIB_SERVER_COMMON_H__

#include <WinSock2.h>
#include <string>
#include <algorithm>
#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024

#include <bitset>
#include <iostream>

enum ServerType : BYTE{
	serverTypeNone = 0,

	serverTypeLogin,
	serverTypeLobby,
	serverTypeGame,
	serverTypeDBManager,
	serverTypeSocial,

	serverTypeCount,
};

/*
*	Enum type operator
*/

// |=
template<typename T_Enum,
	std::enable_if_t<std::is_enum_v<T_Enum> >* = nullptr >
inline T_Enum operator|(T_Enum a, char b) {
	return static_cast<T_Enum>(static_cast<int>(a) | b);
}
template<typename T_Enum,
	std::enable_if_t<std::is_enum_v<T_Enum> >* = nullptr >
inline T_Enum& operator|=(T_Enum& a, char b) {
	return a = a | b;
}

// >>=
template<typename T_Enum,
	std::enable_if_t<std::is_enum_v<T_Enum> >* = nullptr >
inline T_Enum operator>>(T_Enum a, int b) {
	return static_cast<T_Enum>(static_cast<int>(a) >> b);
}
template<typename T_Enum,
	std::enable_if_t<std::is_enum_v<T_Enum> >* = nullptr >
inline T_Enum& operator>>=(T_Enum& a, int b) {
	return a = a >> b;
}

class Buffer {
public:
	Buffer() {}
	
	Buffer(const char* rhs) { 
		memcpy(m_buffer, rhs, sizeof(rhs)); 
		m_length = sizeof(rhs);
	}
	Buffer(char* rhs, size_t sz) { 
		memcpy(m_buffer, rhs, sz);
		m_length = sz;
	}
	Buffer(Buffer& rhs) {
		memcpy(m_buffer, rhs.m_buffer, rhs.m_length);
		m_index = rhs.m_index;
		m_length = rhs.m_length;
	}

	Buffer& operator=(const char* buffer) {
		memcpy(m_buffer, buffer, sizeof(buffer));
		m_length = sizeof(buffer);
		return *this;
	}
//	Buffer& operator=(char* buffer) {
//		memcpy(m_buffer, buffer, sizeof(buffer));
//		m_length = sizeof(buffer);
//		return *this;
//	}
	Buffer& operator=(const Buffer& rhs) {
		if (this == &rhs)
			return *this;

		memcpy(m_buffer, rhs.m_buffer, rhs.m_length);
		m_index = rhs.m_index;
		m_length = rhs.m_length;
		return *this;
	}
	
	
	operator const char*() {
		return this->m_buffer;
	}
	operator char*() {
		return this->m_buffer;
	}
	char& operator[](int index) {
		if (m_length < index)
			return m_buffer[BUFFER_SIZE];

		return m_buffer[index];
	}

#pragma region operator <<
	//1Byte
	Buffer& operator<<(char rhs) {
		m_buffer[m_index] = rhs;
		++m_index;
		++m_length;
		return *this;
	}
	//Enum type
	template<typename T_Arg,
		std::enable_if_t<std::is_enum_v<T_Arg> >* = nullptr >
	Buffer& operator<<(T_Arg rhs) {
		for (int i = 0; i < sizeof(rhs); ++i) {
			m_buffer[m_index] = static_cast<char>(rhs);
			++m_index;
			++m_length;
			rhs >>= 8;
		}
		return *this;
	}
//	Buffer& operator<<(bool rhs) {
//		m_buffer[m_index] = rhs;
//		++m_index;
//		++m_length;
//		return *this;
//	}
	//int
	Buffer& operator<<(__int16& rhs) {
		for (int i = 0; i < sizeof(rhs); ++i) {
			m_buffer[m_index] = static_cast<char>(rhs);
			++m_index;
			++m_length;
			rhs >>= 8;
		}
		return *this;
	}
	Buffer& operator<<(__int32& rhs) {
		for (int i = 0; i < sizeof(rhs); ++i) {
			m_buffer[m_index] = static_cast<char>(rhs);
			++m_index;
			++m_length;
			rhs >>= 8;
		}
		return *this;
	}
	Buffer& operator<<(__int64& rhs) {
		for (int i = 0; i < sizeof(rhs); ++i) {
			m_buffer[m_index] = static_cast<char>(rhs);
			++m_index;
			++m_length;
			rhs >>= 8;
		}
		return *this;
	}
	Buffer& operator<<(size_t& rhs) {
		for (int i = 0; i < sizeof(rhs); ++i) {
			m_buffer[m_index] = static_cast<char>(rhs);
			++m_index;
			++m_length;
			rhs >>= 8;
		}
		return *this;
	}
	//string
	Buffer& operator<<(std::string& rhs) {
		for (int i = 0; i < rhs.size(); ++i) {
			m_buffer[m_index] = rhs[i];
			++m_index;
			++m_length;
		}
		m_buffer[m_index] = '\n';
		++m_length;
		++m_index;
		return *this;
	}
	Buffer& operator<<(const char* rhs) {
		for (int i = 0; i < strlen(rhs); ++i) {
			m_buffer[m_index] = rhs[i];
			++m_index;
			++m_length;
		}
		m_buffer[m_index] = '\n';
		++m_length;
		++m_index;
		return *this;
	}
#pragma endregion
#pragma region operator >>
	Buffer & operator>>(bool& type) {
		if (m_length <= m_index)
			return *this;
		type = m_buffer[m_index];
		++m_index;
		return *this;
	}
	Buffer& operator>>(char& type) {
		if (m_length <= m_index)
			return *this;
		type = m_buffer[m_index];
		++m_index;
		return *this;
	}
	Buffer& operator>>(std::string& str) {
		if (m_length <= m_index)
			return *this;
		str.clear();
		while ('\n' != m_buffer[m_index]) {
			str += m_buffer[m_index];
			++m_index;
		}
		++m_index;
		return *this;
	}
	Buffer& operator>>(__int16& integer) {
		if (m_length <= m_index)
			return *this;
		integer = 0;
		for (int i = 0; i < sizeof(__int16); ++i) {
			__int16 temp = ((m_buffer[m_index] & 0xff) << 8 * i);
			integer |= temp;
			++m_index;
		}
		return *this;
	}
	Buffer& operator>>(__int32& integer) {
		if (m_length <= m_index)
			return *this;
		integer = 0;
		for (int i = 0; i < sizeof(__int32); ++i) {
			__int32 temp = ((m_buffer[m_index] & 0xff) << 8 * i);
			integer |= temp;
			++m_index;
		}
		return *this;
	}
	Buffer& operator>>(__int64& integer) {
		if (m_length <= m_index)
			return *this;
		integer = 0;
		for (int i = 0; i < sizeof(__int64); ++i) {
			__int64 temp = ((m_buffer[m_index] & 0xff) << 8 * i);
			integer |= temp;
			++m_index;
		}
		return *this;
	}

	//Handle deserialize enum value
	template<typename T_Arg, 
		std::enable_if_t<std::is_enum_v<T_Arg> >* = nullptr >
	Buffer& operator>>(T_Arg& integer) {
		if (m_length <= m_index)
			return *this;

		for (int i = 0; i < sizeof(T_Arg); ++i) {
			char temp = (m_buffer[m_index] << 8 * i);
			integer |= temp;
			++m_index;
		}
		return *this;
	}
#pragma endregion

	size_t Length() const { return m_length; }
	void SetLength(size_t sz) { m_length = sz; } //only used when packet recv
	void Reset() {
		std::fill_n(m_buffer, BUFFER_SIZE, '\0');
		m_index = 0;
		m_length = 0;
	}
private:
	char		m_buffer[BUFFER_SIZE] = { '\0', };
	int			m_index = 0;
	size_t		m_length = 0;
};

struct BufferInfo {
	void Clear() {
		ZeroMemory(&(overlapped), sizeof(WSAOVERLAPPED));
		buffer.Reset();
		dataBuf.len = BUFFER_SIZE;
		dataBuf.buf = buffer;
		recvBufferSize = 0;
		sendBufferSize = 0;
	}

	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	Buffer			buffer;
	size_t			recvBufferSize = 0;
	size_t			sendBufferSize = 0;
};

#endif //!__BASELIB_SERVER_COMMON_H__