#include "SocialPlayer.h"

SocialPlayer::SocialPlayer(GPID gpid, std::shared_ptr<ClntServer> server) : 
	m_gpid(gpid), 
	m_server(server) {

}

void SocialPlayer::Initialize() {
	//Load from database
	//friend, guild request
}

