#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include <thread>
#include "PlayerManager.h"
#include "ClntServer.h"
#include "ManagementPacket.h"

class GameServer : public ClntServer {
public:
	GameServer() {}
	GameServer(SOCKET socket) : ClntServer(socket) {}
	~GameServer() {}
	std::string GetChannelName() const {
		return _channel._channelName;
	}
	void SetChannelName(std::string channelName) {
		_channel._channelName = channelName;
	}
	int32 GetCurrentPeople() const {
		return _channel._currentPeople;
	}
	void SetCurrentPeople(__int32 currentPeople) {
		_channel._currentPeople = currentPeople;
	}
	int32 GetMaximumPeople() const {
		return _channel._maximumPeople;
	}
	void SetMaximumPeople(__int32 maximumPeople) {
		_channel._maximumPeople = maximumPeople;
	}
private:
	Channel _channel{ "", 0, 300 };
};


#endif // __GAME_SERVER_H__