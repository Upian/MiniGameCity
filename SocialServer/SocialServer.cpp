#include "SocialServer.h"

SocialServer::SocialServer() {}
SocialServer::~SocialServer() {}

void SocialServer::InitializeSocialServer() {

}

void SocialServer::HandleAcceptClient(SOCKET clientSocket) {
	printf("%lld server connected\n", clientSocket);
}

void SocialServer::HandleDisconnectClient(SOCKET clientSocket) {

}

void SocialServer::HandleBasePacket(BufferInfo* bufInfo) {

}
