#pragma once
#ifndef __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
#define __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__

#include "BaseServer.h"
#include "ClntServerManager.h"

/*
*	
*/

class ManagementServer : public BaseServer<ManagementServer> {
public:


private:
	MAKE_SERVER(ManagementServer);

	//manager
	ClntServerManager m_gameServerManager;
	

};
#endif // !__MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
