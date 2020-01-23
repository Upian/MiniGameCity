#pragma once
#ifndef __BASELIB_SERVER_MANAGER_H__
#define  __BASELIB_SERVER_MANAGER_H__

#include "Singleton.h"

class ServerManager : public Util::Singleton<ServerManager> {
public:

private:
	DECLARE_SINGLETON(ServerManager)

};

#endif // !__BASELIB_SERVER_MANAGER_H__
