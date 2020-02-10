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
	Twenty_GameStart_Timer_On,
	Twenty_GameStart_Timer_Off,
	Twenty_Provide_5Answer,
	/*Twenty_Question_Provide*/
	Twenty_Provider_Select_Answer,
	Twenty_GameStart,
	Twenty_InGameTimer,
	Twenty_Asker_Question,
	Twenty_Asker_Question_BroadCast,
	Twenty_Provider_Reply,
	Twenty_Provider_Reply_Broadcast,
	Twenty_Asker_Answer,
	Twenty_Asker_Answer_Broadcast,
	Twenty_Notice_Next_Asker,
	Twenty_Round_End,
	Twenty_Update_Score,
	Twenty_Answer_Open,
	Twenty_Rest_Time,
	Twenty_Game_End,
	Twenty_Escpse_Quiz_Provider,
	Twenty_Escape_Another_Player,
	Twenty_Exit_Reservation,
	Twenty_Exit_Immediately,
	Twenty_Exit_Notification,
	Twenty_Remain_Question
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
class TwentyPlayerReadyCompletePacket : public TwentyQuestionGamePacket {
public:
	TwentyPlayerReadyCompletePacket() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Answer){}
};
class TwentyCountDownStart : public TwentyQuestionGamePacket {
public:
	TwentyCountDownStart() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart_Timer_On)
	{

	}
};
class TwentyCountDownEnd : public TwentyQuestionGamePacket 
{
public:
	TwentyCountDownEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart_Timer_Off)
	{

	}
};
class TwentyProviderSelectAnswer : public TwentyQuestionGamePacket
{
	TwentyProviderSelectAnswer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provide_5Answer)
	{

	}
};
class TwentySelectAnswer : public TwentyQuestionGamePacket {
	TwentySelectAnswer() :TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Select_Answer)
	{

	}
};
class TwentyGameStart :public TwentyQuestionGamePacket {
	TwentyGameStart() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart)
	{

	}
};
class TwentyInGameTimer : public TwentyQuestionGamePacket {
	TwentyInGameTimer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_InGameTimer)
	{

	}
};
class TwentyAskerQuestion : public TwentyQuestionGamePacket {
	TwentyAskerQuestion() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Question)
	{

	}
};
class TwentyAskerQuestionBroadCast : public TwentyQuestionGamePacket {
	TwentyAskerQuestionBroadCast() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Question_BroadCast)
	{

	}
};
class TwentyAskerQuestion : public TwentyQuestionGamePacket {
	TwentyAskerQuestion() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Question)
	{

	}
};
class TwentyProviderReply : public TwentyQuestionGamePacket {
	TwentyProviderReply() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Reply)
	{

	}
};
class TwentyProviderReplyBroadCast : public TwentyQuestionGamePacket {
	TwentyProviderReplyBroadCast() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Reply_Broadcast)
	{

	}
};
class TwentyAskerAnswer : public TwentyQuestionGamePacket {
	TwentyAskerAnswer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Answer)
	{

	}
};
class TwentyAskerAnswerBroad : public TwentyQuestionGamePacket {
	TwentyAskerAnswerBroad() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Answer_Broadcast)
	{

	}
};
class TwentyNoticeNextAsker : public TwentyQuestionGamePacket {
	TwentyNoticeNextAsker() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Notice_Next_Asker)
	{

	}
};
class TwentyRoundEnd : public TwentyQuestionGamePacket {
	TwentyRoundEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Round_End)
	{

	}
};
class TwentyUpdateScore : public TwentyQuestionGamePacket {
	TwentyUpdateScore() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Update_Score)
	{

	}
};

class TwentyAnswerOpen : public TwentyQuestionGamePacket {
	TwentyAnswerOpen() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Answer_Open)
	{

	}
};
class TwentyRestTime : public TwentyQuestionGamePacket {
	TwentyRestTime() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Rest_Time)
	{

	}
};
class TwentyGameEnd : public TwentyQuestionGamePacket {
	TwentyGameEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Game_End)
	{

	}
};
class TwentyEscapeQuizProvider : public TwentyQuestionGamePacket {
	TwentyEscapeQuizProvider() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escpse_Quiz_Provider)
	{

	}
};
class TwentyEscapeAnotherPlayer : public TwentyQuestionGamePacket {
	TwentyEscapeAnotherPlayer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escape_Another_Player)
	{

	}
};
class TwentyExitReservation : public TwentyQuestionGamePacket {
	TwentyExitReservation() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Reservation)
	{

	}
};
class TwentyExitImmediately : public TwentyQuestionGamePacket {
	TwentyExitImmediately() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Immediately)
	{

	}
};
class TwentyExitNotification : public TwentyQuestionGamePacket {
	TwentyExitNotification() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Notification)
	{

	}
};
class TwentyRemainQuestion : public TwentyQuestionGamePacket {
	TwentyRemainQuestion() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Remain_Question)
	{

	}
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