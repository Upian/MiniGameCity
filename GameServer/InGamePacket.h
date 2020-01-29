#pragma once
#ifndef _IN_GAME_PACKET_H_
#define _IN_GAME_PACKET_H_

#include <string>

#include "BasePacket.h"
#include "Player.h"

enum Twenty_Packet_Name
{
	//수정중
	Twenty_None,
	Twenty_Player_Location_Setting,
	Twenty_Player_Ready_Complete,
	Twenty_First_Timer_Start,
	Twenty_First_Timer_End,
	Twenty_Question_Provide,
	Twenty_Remain_Time_Check,
	Twenty_Ask_Time_Over,
	Twenty_Asker_Question,
	Twenty_Asker_Question_Broadcast,
	Twenty_Provider_Reply,
	Twenty_Provider_Reply_Broadcast,
	Twenty_Asker_Answer,
	Twenty_Asker_Answer_Correct,
	Twenty_Asker_Answer_Wrong,
	Twenty_Question_Chance_Over,
	Twenty_Provider_Reply_Time_Over
};
enum Relay_Packet_Name
{
	//제작중
	Relay_None,
	Relay_Start_Response,
	Relay_Sentens_Request,
	Relay_Sentens_Response,
	Relay_Score_Request,
	Relay_Next_Response,
	Relay_Timer
};
enum Ban_Packet_Name
{
	//제작중
	Ban_None,
	Ban_Start_Response,
	Ban_Set_Request,
	Ban_Round_Start_Response,
	Ban_Chat_Request,
	Ban_Banner_Set_Responce,
	Ban_End_Game_Responce,
	Ban_Timer
};
enum Catch_Pakcet_Name
{
	//제작중
	Catch_None,
	Catch_Start_Response,
	Catch_Quiz_Answer_Response,
	Catch_Paint_Response,
	Catch_Paint_Request,
	Catch_Chat_Response,
	Catch_Chat_Request,
	Catch_Timer
};

class InGamePacket : public BasePacket {
public:
	InGamePacket() {}
	InGamePacket(BasePacketType _base_packet_type)
	{
		this->base_packet_type = _base_packet_type;
	}
	virtual ~InGamePacket() {}
private:
	BasePacketType base_packet_type = basePacketTypeNone;
};

#endif