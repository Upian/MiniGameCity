#include "DBCache.h"

DBCache::DBCache() {}
DBCache::~DBCache() {}

void DBCache::HandleAcceptClient(SOCKET clientSocket) {

}
void DBCache::HandleDisconnectClient(SOCKET clientScoket) {

}

void DBCache::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo) return;

	Util::LoggingInfo("DBCache.log", "Recv packet size: %d", bufInfo->buffer.Length());
	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::managementPacketTypeDBCache: {
		this->HandlePacketLogin(bufInfo);
		break;
	}
	default: {
		Util::LoggingInfo("DBCache.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
	printf("DBCache -- recv packet: %s\n", bufInfo->buffer);

}

void DBCache::HandlePacketLogin(BufferInfo* bufInfo) {
	DBCachePacketType type = (DBCachePacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case managementDBCachePacketTypeLoginRequest: {
		ManagementDBCachePacketTypeLoginRequest packetManagementRequest{};
		packetManagementRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeLoginRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((Util::GetConfigToInt("DBCache.ini", "Definition", "UserIdMinSize", 4) > packetManagementRequest.userId.size()) || (packetManagementRequest.userId.size() > Util::GetConfigToInt("DBCache.ini", "Definition", "UserIdMaxSize", 8))) {
			flag = false;
		}
		if ((flag == true) && (Util::GetConfigToInt("DBCache.ini", "Definition", "UserPwMinSize", 8) > packetManagementRequest.userPw.size()) || (packetManagementRequest.userPw.size() > Util::GetConfigToInt("DBCache.ini", "Definition", "UserPwMaxSize", 16))) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetManagementRequest.userId.size(); ++i) {
				if (!isalnum(packetManagementRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetManagementRequest.userPw.size(); ++i) {
				if (!isalnum(packetManagementRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}

		if (flag == true) {
			ManagementDBCachePacketTypeLoginResponse packetManagementResponse{};

			DatabaseAPI db;
			if (db.Connect(Util::GetConfigToString("DBCache.ini", "DB", "Host", "127.0.0.1"), 
				           Util::GetConfigToString("DBCache.ini", "DB", "User", "root"), 
				           Util::GetConfigToString("DBCache.ini", "DB", "Pass", "minicity1!"), 
				           Util::GetConfigToString("DBCache.ini", "DB", "Name", "minigamecity"), 
				           Util::GetConfigToInt("DBCache.ini", "DB", "Port", 3306))) {

				printf("db 立加 己傍\n");
				Account account = db.LoginAccount(packetManagementRequest.userId, packetManagementRequest.userPw);
				packetManagementResponse.flag = account.flag;
				packetManagementResponse.userNick = account.nick;
				packetManagementResponse.GPID = db.GetGpid();
				db.Close();
			}
			else {
				printf("db 立加 角菩\n");
				packetManagementResponse.flag = false;
				packetManagementResponse.userNick = "";
				packetManagementResponse.GPID = 0;
			}

			bufInfo->Clear();
			bufInfo->buffer = packetManagementResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeLoginResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		else {
			ManagementDBCachePacketTypeLoginResponse packetManagementResponse{};
			packetManagementResponse.flag = false;
			packetManagementResponse.userNick = "";
			packetManagementResponse.GPID = 0;
			bufInfo->Clear();
			bufInfo->buffer = packetManagementResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeLoginResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		break;
	}
	case managementDBCachePacketTypeLogoutRequest: {
		ManagementDBCachePacketTypeLogoutRequest packetManagementRequest{};
		packetManagementRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeLogoutRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		DatabaseAPI db;
		db.SetGpid(packetManagementRequest.GPID);
		if (db.Connect(Util::GetConfigToString("DBCache.ini", "DB", "Host", "127.0.0.1"),
			           Util::GetConfigToString("DBCache.ini", "DB", "User", "root"),
			           Util::GetConfigToString("DBCache.ini", "DB", "Pass", "minicity1!"),
			           Util::GetConfigToString("DBCache.ini", "DB", "Name", "minigamecity"),
			           Util::GetConfigToInt("DBCache.ini", "DB", "Port", 3306))) {
			printf("db 立加 己傍\n");
			db.LogoutAccount();
			db.Close();
		}
		else {
			printf("db 立加 角菩\n");
		}
		break;
	}

	case managementDBCachePacketTypeSignupRequest: {
		ManagementDBCachePacketTypeSignupRequest packetManagementRequest{};
		packetManagementRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeSignupRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((Util::GetConfigToInt("DBCache.ini", "Definition", "UserIdMinSize", 4) > packetManagementRequest.userId.size()) || (packetManagementRequest.userId.size() > Util::GetConfigToInt("DBCache.ini", "Definition", "UserIdMaxSize", 8))) {
			flag = false;
		}
		if ((flag == true) && (Util::GetConfigToInt("DBCache.ini", "Definition", "UserPwMinSize", 8) > packetManagementRequest.userPw.size()) || (packetManagementRequest.userPw.size() > Util::GetConfigToInt("DBCache.ini", "Definition", "UserPwMaxSize", 16))) {
			flag = false;
		}
		if ((flag == true) && (packetManagementRequest.userNick.size() > Util::GetConfigToInt("DBCache.ini", "Definition", "UserNickMaxSize", 24))) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetManagementRequest.userId.size(); ++i) {
				if (!isalnum(packetManagementRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetManagementRequest.userPw.size(); ++i) {
				if (!isalnum(packetManagementRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}

		if (flag == true) {
			ManagementDBCachePacketTypeSignupResponse packetManagementResponse{};
			DatabaseAPI db;
			if (db.Connect(Util::GetConfigToString("DBCache.ini", "DB", "Host", "127.0.0.1"),
				           Util::GetConfigToString("DBCache.ini", "DB", "User", "root"),
				           Util::GetConfigToString("DBCache.ini", "DB", "Pass", "minicity1!"),
				           Util::GetConfigToString("DBCache.ini", "DB", "Name", "minigamecity"),
				           Util::GetConfigToInt("DBCache.ini", "DB", "Port", 3306))) {
				printf("db 立加 己傍\n");
				packetManagementResponse.flag = db.SignUpAccount(packetManagementRequest.userId, packetManagementRequest.userPw, packetManagementRequest.userNick);
				db.Close();
			}
			else {
				printf("db 立加 角菩\n");
				packetManagementResponse.flag = false;
			}
			
			bufInfo->Clear();
			bufInfo->buffer = packetManagementResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeSignupResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		else {
			ManagementDBCachePacketTypeSignupResponse packetManagementResponse{};
			packetManagementResponse.flag = false;
			bufInfo->Clear();
			bufInfo->buffer = packetManagementResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeSignupResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}

	case managementDBCachePacketTypeDeleteRequest: {
		ManagementDBCachePacketTypeDeleteRequest packetManagementRequest{};
		packetManagementRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeDeleteRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		ManagementDBCachePacketTypeDeleteResponse packetManagementResponse{};
		DatabaseAPI db;
		db.SetGpid(packetManagementRequest.GPID);
		if (db.Connect(Util::GetConfigToString("DBCache.ini", "DB", "Host", "127.0.0.1"),
			           Util::GetConfigToString("DBCache.ini", "DB", "User", "root"),
			           Util::GetConfigToString("DBCache.ini", "DB", "Pass", "minicity1!"),
			           Util::GetConfigToString("DBCache.ini", "DB", "Name", "minigamecity"),
			           Util::GetConfigToInt("DBCache.ini", "DB", "Port", 3306))) {
			printf("db 立加 己傍\n");
			packetManagementResponse.flag = db.StopAccount();
			db.Close();
		}
		else {
			printf("db 立加 角菩\n");
			packetManagementResponse.flag = false;
		}
		bufInfo->Clear();
		bufInfo->buffer = packetManagementResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("DBCache.log", "Type : ManagementDBCachePacketTypeDeleteResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	default: {
		Util::LoggingInfo("DBCache.log", "Recv wrong login packet ID : %d", type);
	}
	}
}