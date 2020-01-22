#pragma once
#ifndef __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
#define __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__

#include "BaseServer.h"
/*
*	
*/

class ManagementServer : public BaseServer<ManagementServer> {
public:


private:
	MAKE_SERVER(ManagementServer);

	std::list<SOCKADDR_IN> m_gameServerList;
	

};
#endif // !__MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
