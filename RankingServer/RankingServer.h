#pragma once
#ifndef __RANKINGSERVER_RANKING_SERVER_H__
#define __RANKINGSERVER_RANKING_SERVER_H__

#include "BaseServer.h"

class RankingServer : public BaseServer<RankingServer> {
public:

private:
	MAKE_SERVER(RankingServer)

};

#endif // !__RANKINGSERVER_RANKING_SERVER_H__


