#pragma once
#ifndef _IN_GAME_PACKET_H_
#define _IN_GAME_PACKET_H_

#include <string>

#include "BasePacket.h"
#include "Player.h"

enum InGamePacketType
{
	InGame_Packet_None,
	Twenty_Question_Game,
	Relay_Novels_Writing_Game,
	Ban_Keyword_Game,
	Catch_Mind_Game
};

enum Twenty_Packet_Type
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
enum Relay_Packet_Type
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
enum Ban_Packet_Type
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
enum Catch_Pakcet_Type
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

//Ingame Base Packet
class InGamePacket : public BasePacket {
public:
	InGamePacket(InGamePacketType _ingame_packet_type) : BasePacket(BasePacketType::basePacketTypeGame), ingame_packet_type(_ingame_packet_type)
	{
		this->TypeSerial(ingame_packet_type);
	}
	~InGamePacket() {}
	InGamePacketType GetInGamePacketType() { return ingame_packet_type; }
	void SetInGamePacketType(InGamePacketType _ingame_packet_type) { ingame_packet_type = _ingame_packet_type; }
protected:
	InGamePacketType ingame_packet_type = InGame_Packet_None;
};

//Twenty Base Packet
class TwentyQuestionGamePacket : public InGamePacket {
public:
	TwentyQuestionGamePacket(Twenty_Packet_Type _twenty_packet) : InGamePacket(InGamePacketType::Twenty_Question_Game), twenty_packet(_twenty_packet)
	{
		this->TypeSerial(_twenty_packet);
	}
	~TwentyQuestionGamePacket();
protected:
	Twenty_Packet_Type twenty_packet = Twenty_None;
};
class TwentyPlayerLocationSettingPacket : public TwentyQuestionGamePacket {
public:
	TwentyPlayerLocationSettingPacket() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Player_Location_Setting)
	{
		std::string Player_ID[5];
		int Player_Count;
	}
	~TwentyPlayerLocationSettingPacket();
private:

};
class RelayNovelWritingGamePacket : public InGamePacket {

};
class BanKeywordGamePacket : public InGamePacket {

};
class CatchMianGamePacket : public InGamePacket {

};
struct Player_Location_Setting_Packet : public InGamePacket
{
	int packet_type = Twenty_Player_Location_Setting;
};
#endif