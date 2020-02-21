#include "LoginDBCache.h"

LoginDBCache::LoginDBCache() {}
LoginDBCache::~LoginDBCache() {}

void LoginDBCache::HandleAcceptClient(SOCKET clientSocket) {

}
void LoginDBCache::HandleDisconnectClient(SOCKET clientScoket) {

}

void LoginDBCache::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo) return;

	Util::LoggingInfo("LoginDBCache.log", "Recv packet size: %d", bufInfo->buffer.Length());
	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::loginPacketTypeDBCache: {
		this->HandlePacketLogin(bufInfo);
		break;
	}
	default: {
		Util::LoggingInfo("LoginDBCache.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
	printf("LoginDBCache -- recv packet: %s\n", bufInfo->buffer);

}

void LoginDBCache::HandlePacketLogin(BufferInfo* bufInfo) {
	DBCachePacketType type = (DBCachePacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case loginDBCachePacketTypeLoginRequest: {
		LoginDBCachePacketTypeLoginRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeLoginRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserIdMinSize", 4) > packetLoginRequest.userId.size()) || (packetLoginRequest.userId.size() > Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserIdMaxSize", 8))) {
			flag = false;
		}
		if ((flag == true) && (Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserPwMinSize", 8) > packetLoginRequest.userPw.size()) || (packetLoginRequest.userPw.size() > Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserPwMaxSize", 16))) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetLoginRequest.userId.size(); ++i) {
				if (!isalnum(packetLoginRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetLoginRequest.userPw.size(); ++i) {
				if (!isalnum(packetLoginRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}

		if (flag == true) {
			LoginDBCachePacketTypeLoginResponse packetLoginResponse{};

			// test dummy
			if (packetLoginRequest.userId == "admin1" && packetLoginRequest.userPw == "12341234") {
				packetLoginResponse.flag = true;
				packetLoginResponse.userNick = "1일1일";
				packetLoginResponse.gpid = 1;
				packetLoginResponse.userId = packetLoginRequest.userId;
				bufInfo->Clear();
				bufInfo->buffer = packetLoginResponse.Serialize();
				send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			}
			else if (packetLoginRequest.userId == "admin2" && packetLoginRequest.userPw == "12341234") {
				packetLoginResponse.flag = true;
				packetLoginResponse.userNick = "2이2이";
				packetLoginResponse.gpid = 2;
				packetLoginResponse.userId = packetLoginRequest.userId;
				bufInfo->Clear();
				bufInfo->buffer = packetLoginResponse.Serialize();
				send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			}
			else if (packetLoginRequest.userId == "admin3" && packetLoginRequest.userPw == "12341234") {
				packetLoginResponse.flag = true;
				packetLoginResponse.userNick = "3삼3삼";
				packetLoginResponse.gpid = 3;
				packetLoginResponse.userId = packetLoginRequest.userId;
				bufInfo->Clear();
				bufInfo->buffer = packetLoginResponse.Serialize();
				send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			}
			else if (packetLoginRequest.userId == "admin4" && packetLoginRequest.userPw == "12341234") {
				packetLoginResponse.flag = true;
				packetLoginResponse.userNick = "4사4사";
				packetLoginResponse.gpid = 4;
				packetLoginResponse.userId = packetLoginRequest.userId;
				bufInfo->Clear();
				bufInfo->buffer = packetLoginResponse.Serialize();
				send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			}
			else if (packetLoginRequest.userId == "admin5" && packetLoginRequest.userPw == "12341234") {
				packetLoginResponse.flag = true;
				packetLoginResponse.userNick = "5오5오";
				packetLoginResponse.gpid = 5;
				packetLoginResponse.userId = packetLoginRequest.userId;
				bufInfo->Clear();
				bufInfo->buffer = packetLoginResponse.Serialize();
				send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			}
			else if (packetLoginRequest.userId == "admin6" && packetLoginRequest.userPw == "12341234") {
				packetLoginResponse.flag = true;
				packetLoginResponse.userNick = "6육6육";
				packetLoginResponse.gpid = 6;
				packetLoginResponse.userId = packetLoginRequest.userId;
				bufInfo->Clear();
				bufInfo->buffer = packetLoginResponse.Serialize();
				send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			}
			else {
				DatabaseAPI db;
				if (db.Connect(Util::GetConfigToString("LoginDBCache.ini", "DB", "Host", "127.0.0.1"),
					Util::GetConfigToString("LoginDBCache.ini", "DB", "User", "root"),
					Util::GetConfigToString("LoginDBCache.ini", "DB", "Pass", "minicity1!"),
					Util::GetConfigToString("LoginDBCache.ini", "DB", "Name", "minigamecity"),
					Util::GetConfigToInt("LoginDBCache.ini", "DB", "Port", 3306))) {

					printf("db 접속 성공\n");
					Account account = db.LoginAccount(packetLoginRequest.userId, packetLoginRequest.userPw);
					packetLoginResponse.flag = account.flag;
					packetLoginResponse.userNick = account.nick;
					packetLoginResponse.gpid = db.GetGpid();
					packetLoginResponse.userId = packetLoginRequest.userId;
					// 중복되어 패킷보내야 한다.
					if (account.duplicate == true) {
						LoginDBCachePacketTypeDisconnectUserRequest packet;
						packet.gpid = db.GetGpid();
						bufInfo->Clear();
						bufInfo->buffer = packet.Serialize();
						send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
					}
					db.Close();
				}
				else {
					printf("db 접속 실패\n");
					packetLoginResponse.flag = false;
					packetLoginResponse.userNick = "";
					packetLoginResponse.gpid = 0;
					packetLoginResponse.userId = packetLoginRequest.userId;
				}

				bufInfo->Clear();
				bufInfo->buffer = packetLoginResponse.Serialize();
				send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
				Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeLoginResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

			}	
		}
		else {
			LoginDBCachePacketTypeLoginResponse packetLoginResponse{};
			packetLoginResponse.flag = false;
			packetLoginResponse.userNick = "";
			packetLoginResponse.gpid = 0;
			packetLoginResponse.userId = packetLoginRequest.userId;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeLoginResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		break;
	}
	case loginDBCachePacketTypeLogoutRequest: {
		LoginDBCachePacketTypeLogoutRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeLogoutRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		DatabaseAPI db;
		db.SetGpid(packetLoginRequest.gpid);
		if (db.Connect(Util::GetConfigToString("LoginDBCache.ini", "DB", "Host", "127.0.0.1"),
			Util::GetConfigToString("LoginDBCache.ini", "DB", "User", "root"),
			Util::GetConfigToString("LoginDBCache.ini", "DB", "Pass", "minicity1!"),
			Util::GetConfigToString("LoginDBCache.ini", "DB", "Name", "minigamecity"),
			Util::GetConfigToInt("LoginDBCache.ini", "DB", "Port", 3306))) {
			printf("db 접속 성공\n");
			db.LogoutAccount();
			db.Close();
		}
		else {
			printf("db 접속 실패\n");
		}
		break;
	}

	case loginDBCachePacketTypeSignupRequest: {
		LoginDBCachePacketTypeSignupRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeSignupRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserIdMinSize", 4) > packetLoginRequest.userId.size()) || (packetLoginRequest.userId.size() > Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserIdMaxSize", 8))) {
			flag = false;
		}
		if ((flag == true) && (Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserPwMinSize", 8) > packetLoginRequest.userPw.size()) || (packetLoginRequest.userPw.size() > Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserPwMaxSize", 16))) {
			flag = false;
		}
		if ((flag == true) && (packetLoginRequest.userNick.size() > Util::GetConfigToInt("LoginDBCache.ini", "Definition", "UserNickMaxSize", 24))) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetLoginRequest.userId.size(); ++i) {
				if (!isalnum(packetLoginRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetLoginRequest.userPw.size(); ++i) {
				if (!isalnum(packetLoginRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}

		if (flag == true) {
			LoginDBCachePacketTypeSignupResponse packetLoginResponse{};
			DatabaseAPI db;
			if (db.Connect(Util::GetConfigToString("LoginDBCache.ini", "DB", "Host", "127.0.0.1"),
				Util::GetConfigToString("LoginDBCache.ini", "DB", "User", "root"),
				Util::GetConfigToString("LoginDBCache.ini", "DB", "Pass", "minicity1!"),
				Util::GetConfigToString("LoginDBCache.ini", "DB", "Name", "minigamecity"),
				Util::GetConfigToInt("LoginDBCache.ini", "DB", "Port", 3306))) {
				printf("db 접속 성공\n");
				packetLoginResponse.flag = db.SignUpAccount(packetLoginRequest.userId, packetLoginRequest.userPw, packetLoginRequest.userNick);
				packetLoginResponse.gpid = 0;
				packetLoginResponse.userId = packetLoginRequest.userId;
				db.Close();
			}
			else {
				printf("db 접속 실패\n");
				packetLoginResponse.flag = false;
				packetLoginResponse.gpid = 0;
				packetLoginResponse.userId = packetLoginRequest.userId;
			}

			bufInfo->Clear();
			bufInfo->buffer = packetLoginResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeSignupResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		else {
			LoginDBCachePacketTypeSignupResponse packetLoginResponse{};
			packetLoginResponse.flag = false;
			packetLoginResponse.gpid = 0;
			packetLoginResponse.userId = packetLoginRequest.userId;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeSignupResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}

	case loginDBCachePacketTypeDeleteRequest: {
		LoginDBCachePacketTypeDeleteRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeDeleteRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginDBCachePacketTypeDeleteResponse packetLoginResponse{};
		DatabaseAPI db;
		db.SetGpid(packetLoginRequest.gpid);
		if (db.Connect(Util::GetConfigToString("LoginDBCache.ini", "DB", "Host", "127.0.0.1"),
			Util::GetConfigToString("LoginDBCache.ini", "DB", "User", "root"),
			Util::GetConfigToString("LoginDBCache.ini", "DB", "Pass", "minicity1!"),
			Util::GetConfigToString("LoginDBCache.ini", "DB", "Name", "minigamecity"),
			Util::GetConfigToInt("LoginDBCache.ini", "DB", "Port", 3306))) {
			printf("db 접속 성공\n");
			packetLoginResponse.flag = db.StopAccount();
			packetLoginResponse.gpid = db.GetGpid();
			db.Close();
		}
		else {
			printf("db 접속 실패\n");
			packetLoginResponse.flag = false;
			packetLoginResponse.gpid = db.GetGpid();
		}
		bufInfo->Clear();
		bufInfo->buffer = packetLoginResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginDBCache.log", "Type : LoginDBCachePacketTypeDeleteResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	default: {
		Util::LoggingInfo("LoginDBCache.log", "Recv wrong login packet ID : %d", type);
	}
	}
}