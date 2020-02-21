#include "LoginDBCacheHandler.h"
#include "LoginServer.h"
#include "Player.h"
#include "Log.h"
#include <mutex>

void LoginDBCacheHandler::Initialize() {
	m_loginServer = LoginServer::GetServer();
}

/*

		  Client -> Login server

*/

void LoginDBCacheHandler::HandleLoginPacket(Buffer& buffer, std::shared_ptr<Player> player, LoginPacketType type) {
	if (player == nullptr)
		return;

	switch (type) {
	case LoginPacketType::clientLoginPacketTypeLoginRequest: {
		//Util::LoggingInfo("LoginServer.log", "Type : LoginDBCachePacketTypeLoginResponse || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		ClientLoginPacketTypeLoginRequest packetClientRequest{};
		packetClientRequest.Deserialize(buffer);
		player->SetName(packetClientRequest.userId);
		std::unique_lock<std::mutex> lck(mtx);
		lck.lock();
		player->SetSessionID(std::time(nullptr));
		lck.unlock();
		if (CheckIDPW(packetClientRequest.userId, packetClientRequest.userPw)) {
			this->HandlePacketLoginRequest(packetClientRequest, player);
		}
		else {
			LoginDBCachePacketTypeLoginResponse packetClientResponse{};
			player->SetName(packetClientRequest.userId);
			packetClientResponse.flag = false;
			this->HandlePacketLoginResponse(packetClientResponse, player);
		}
		break;
	}
	case LoginPacketType::clientLoginPacketTypeLogoutRequest: {
		ClientLoginPacketTypeLogoutRequest packetClientRequest{};
		packetClientRequest.Deserialize(buffer);
		this->HandlePacketLogoutRequest(packetClientRequest, player);
		break;
	}
	case LoginPacketType::clientLoginPacketTypeSignupRequest: {
		ClientLoginPacketTypeSignupRequest packetClientRequest{};
		packetClientRequest.Deserialize(buffer);
		player->SetName(packetClientRequest.userId);
		if (CheckIDPW(packetClientRequest.userId, packetClientRequest.userPw) && CheckNick(packetClientRequest.userNick)) {
			this->HandlePacketSignupRequest(packetClientRequest, player);
		}
		else {
			LoginDBCachePacketTypeSignupResponse packetClientResponse{};
			player->SetName(packetClientRequest.userId);
			packetClientResponse.flag = false;
			this->HandlePacketSignupResponse(packetClientResponse, player);
		}
		break;
	}
	case LoginPacketType::clientLoginPacketTypeDeleteRequest: {
		ClientLoginPacketTypeDeleteRequest packetClientRequest{};
		packetClientRequest.Deserialize(buffer);
		this->HandlePacketDeleteRequest(packetClientRequest, player);
		break;
	}
	default:
		break;
	}
}

/*

		  Login Server -> DBCache Server

*/

void LoginDBCacheHandler::HandlePacketLoginRequest(ClientLoginPacketTypeLoginRequest& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	LoginDBCachePacketTypeLoginRequest packetLoginRequest{};
	packetLoginRequest.userId = packet.userId;
	packetLoginRequest.userPw = packet.userPw;
	this->SendPacketToServer(packetLoginRequest);
}

void LoginDBCacheHandler::HandlePacketLogoutRequest(ClientLoginPacketTypeLogoutRequest& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	LoginDBCachePacketTypeLogoutRequest packetLoginRequest{};
	packetLoginRequest.gpid = player->GetGPID();
	this->SendPacketToServer(packetLoginRequest);
}
void LoginDBCacheHandler::HandlePacketSignupRequest(ClientLoginPacketTypeSignupRequest& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	LoginDBCachePacketTypeSignupRequest packetLoginRequest{};
	packetLoginRequest.userId = packet.userId;
	packetLoginRequest.userPw = packet.userPw;
	packetLoginRequest.userNick = packet.userNick;
	this->SendPacketToServer(packetLoginRequest);
}
void LoginDBCacheHandler::HandlePacketDeleteRequest(ClientLoginPacketTypeDeleteRequest& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	LoginDBCachePacketTypeDeleteRequest packetLoginRequest{};
	packetLoginRequest.gpid = player->GetGPID();
	this->SendPacketToServer(packetLoginRequest);
}

/*

		  DBCache -> Login server

*/

void LoginDBCacheHandler::HandlePacket(Buffer& buffer) {
	BasePacketType type = (BasePacketType)PacketTypeDeserial(buffer);
	if (type != BasePacketType::loginPacketTypeDBCache)
		return;

	DBCachePacketType managementType = (DBCachePacketType)PacketTypeDeserial(buffer);

	switch (managementType) {
	case DBCachePacketType::loginDBCachePacketTypeLoginResponse: {
		//Util::LoggingInfo("LoginServer.log", "Type : LoginDBCachePacketTypeLoginResponse || Recv packet : %s || size: %d", buffer, buffer.Length());
		LoginDBCachePacketTypeLoginResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(buffer);
		// 기존 로그인 유저 연결 끊기
		auto player = m_loginServer->GetPlayerManager().FindPlayerByName(packetLoginResponse.userId);
		this->HandlePacketLoginResponse(packetLoginResponse, player);
		break;
	}
	case DBCachePacketType::loginDBCachePacketTypeSignupResponse: {
		LoginDBCachePacketTypeSignupResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(buffer);
		auto player = m_loginServer->GetPlayerManager().FindPlayerByName(packetLoginResponse.userId);
		this->HandlePacketSignupResponse(packetLoginResponse, player);
		break;
	}
	case DBCachePacketType::loginDBCachePacketTypeDeleteResponse: {
		LoginDBCachePacketTypeDeleteResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(buffer);
		auto player = m_loginServer->GetPlayerManager().FindPlayer(packetLoginResponse.gpid);
		this->HandlePacketDeleteResponse(packetLoginResponse, player);
		break;
	}
	default:
		break;
	}
}

/*

		  Login Server -> Client

*/

void LoginDBCacheHandler::HandlePacketLoginResponse(LoginDBCachePacketTypeLoginResponse& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;
	player->SetGPID(packet.gpid);
	ClientLoginPacketTypeLoginResponse packetClientResponse{};
	packetClientResponse.flag = packet.flag;
	packetClientResponse.userNick = packet.userNick;
	packetClientResponse.token = player->GetToekn();
	player->SendPacket(packetClientResponse);
}

void LoginDBCacheHandler::HandlePacketSignupResponse(LoginDBCachePacketTypeSignupResponse& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	ClientLoginPacketTypeSignupResponse packetClientResponse{};
	packetClientResponse.flag = packet.flag;
	player->SendPacket(packetClientResponse);
}

void LoginDBCacheHandler::HandlePacketDeleteResponse(LoginDBCachePacketTypeDeleteResponse& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	ClientLoginPacketTypeDeleteResponse packetClientResponse{};
	packetClientResponse.flag = packet.flag;
	player->SendPacket(packetClientResponse);
}

bool LoginDBCacheHandler::CheckIDPW(std::string userId, std::string userPw) {
	bool flag = true;
	if ((Util::GetConfigToInt("LoginServer.ini", "Definition", "UserIdMinSize", 4) > userId.size()) || (userId.size() > Util::GetConfigToInt("LoginServer.ini", "Definition", "UserIdMaxSize", 8))) {
		flag = false;
	}
	if ((flag == true) && (Util::GetConfigToInt("LoginServer.ini", "Definition", "UserPwMinSize", 8) > userPw.size()) || (userPw.size() > Util::GetConfigToInt("LoginServer.ini", "Definition", "UserPwMaxSize", 16))) {
		flag = false;
	}

	if (flag == true) {
		for (int i = 0; i < userId.size(); ++i) {
			if (!isalnum(userId[i])) {
				flag = false;
				break;
			}
		}
		for (int i = 0; i < userPw.size(); ++i) {
			if (!isalnum(userPw[i])) {
				flag = false;
				break;
			}
		}
	}
	return flag;
}

/*

		  Check

*/
bool LoginDBCacheHandler::CheckNick(std::string userNick) {
	bool flag = true;
	if ((flag == true) && (userNick.size() > Util::GetConfigToInt("LoginServer.ini", "Definition", "UserNickMaxSize", 24))) {
		flag = false;
	}
	return flag;
}
