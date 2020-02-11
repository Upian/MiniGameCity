#pragma once
#ifndef __BASELIB_SERVER_MANAGER_H__
#define  __BASELIB_SERVER_MANAGER_H__
#include <list>
#include <memory>

#include "ClntServer.h"



class ClntServerManager  {
public:
	void InsertServer(SOCKET socket);
	
	bool isExistServer(SOCKET socket);
	bool isExistServer(std::shared_ptr<ClntServer> player);
private:
	std::list<std::shared_ptr<ClntServer> > m_servers;

};

#endif // !__BASELIB_SERVER_MANAGER_H__
