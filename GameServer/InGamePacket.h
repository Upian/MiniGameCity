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
		this->PacketTypeSerial(ingame_packet_type);
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
	TwentyPlayerLocationSettingPacket() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Player_Location_Setting) {}
	~TwentyPlayerLocationSettingPacket();

	std::string PlayerName[5] = { NULL, };
	int PlayerCount = 0;

	virtual Buffer& Serialize() override {
		buffer << PlayerName[0];
		buffer << PlayerName[1];
		buffer << PlayerName[2];
		buffer << PlayerName[3];
		buffer << PlayerName[4];
		buffer << PlayerCount;

		return buffer;
	}

	virtual void Deserialize(Buffer& _buf) override {
		_buf >> PlayerName[0];
		_buf >> PlayerName[1];
		_buf >> PlayerName[2];
		_buf >> PlayerName[3];
		_buf >> PlayerName[4];
		_buf >> PlayerCount;
	}
private:

};
class TwentyPlayerReadyCompletePacket : public TwentyQuestionGamePacket {
public:
	TwentyPlayerReadyCompletePacket() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Player_Ready_Complete){}
	~TwentyPlayerReadyCompletePacket() {};

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}

	bool PlayerReadyCheck(Buffer& buf)
	{
		char BaseType;
		InGamePacketType InGameType;
		Twenty_Packet_Type CheckType;

		buf >> BaseType;
		buf >> InGameType;
		buf >> CheckType;
		
		if (CheckType == Twenty_Player_Ready_Complete)
			return true;
		else
			return false;
	}
};
class TwentyCountDownStart : public TwentyQuestionGamePacket {
public:
	TwentyCountDownStart() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart_Timer_On) {}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}

};
class TwentyCountDownEnd : public TwentyQuestionGamePacket 
{
public:
	TwentyCountDownEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart_Timer_Off){}
	
	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyProviderSelectAnswer : public TwentyQuestionGamePacket
{
public:
	TwentyProviderSelectAnswer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provide_5Answer){}

	std::string AnswerChoice[5];

	virtual Buffer& Serialize() override {
		for (std::string s : AnswerChoice)
		{
			buffer << s;
		}
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> AnswerChoice[0];
		buf >> AnswerChoice[1];
		buf >> AnswerChoice[2];
		buf >> AnswerChoice[3];
		buf >> AnswerChoice[4];
	}
};
class TwentySelectAnswer : public TwentyQuestionGamePacket {
public:
	TwentySelectAnswer() :TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Select_Answer){}

	std::string Answer;

	virtual Buffer& Serialize() override {

		buffer << Answer;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> Answer;
	}

	bool AnswerCheck(Buffer& buf)
	{
		char BaseType;
		InGamePacketType InGameType;
		Twenty_Packet_Type packetType;

		buf >> BaseType;
		buf >> InGameType;
		buf >> packetType;
		
		if (packetType == Twenty_Provider_Select_Answer)
		{
			buf >> Answer;
			return true;
		}
		else
			return false;
	}
};
class TwentyGameStart :public TwentyQuestionGamePacket {
public:
	TwentyGameStart() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyInGameTimer : public TwentyQuestionGamePacket {
public:
	TwentyInGameTimer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_InGameTimer){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyAskerQuestion : public TwentyQuestionGamePacket {
public:
	TwentyAskerQuestion() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Question) {}

	std::string Question = 0;

	virtual Buffer& Serialize() override {

		buffer << Question;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> Question;
	}
};
class TwentyAskerQuestionBroadCast : public TwentyQuestionGamePacket {
public:
	TwentyAskerQuestionBroadCast() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Question_BroadCast){}

	std::string PlayerName = 0;
	std::string Question = 0;

	virtual Buffer& Serialize() override {

		buffer << PlayerName;
		buffer << Question;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> PlayerName;
		buf >> Question;

	}
};
class TwentyProviderReply : public TwentyQuestionGamePacket {
public:
	TwentyProviderReply() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Reply){}

	int ReplyOX = 0;

	virtual Buffer& Serialize() override {

		buffer << ReplyOX;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> ReplyOX;
	}
};
class TwentyProviderReplyBroadCast : public TwentyQuestionGamePacket {
public:
	TwentyProviderReplyBroadCast() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Reply_Broadcast){}

	std::string PlayerName;
	int ReplyOX = 0;

	virtual Buffer& Serialize() override {

		buffer << PlayerName;
		buffer << ReplyOX;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> PlayerName;
		buf >> ReplyOX;
	}
};
class TwentyAskerAnswer : public TwentyQuestionGamePacket {
public:
	TwentyAskerAnswer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Answer){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyAskerAnswerBroad : public TwentyQuestionGamePacket {
public:
	TwentyAskerAnswerBroad() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Answer_Broadcast){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyNoticeNextAsker : public TwentyQuestionGamePacket {
public:
	TwentyNoticeNextAsker() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Notice_Next_Asker){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyRoundEnd : public TwentyQuestionGamePacket {
public:
	TwentyRoundEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Round_End){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyUpdateScore : public TwentyQuestionGamePacket {
public:
	TwentyUpdateScore() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Update_Score){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};

class TwentyAnswerOpen : public TwentyQuestionGamePacket {
public:
	TwentyAnswerOpen() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Answer_Open){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyRestTime : public TwentyQuestionGamePacket {
public:
	TwentyRestTime() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Rest_Time){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyGameEnd : public TwentyQuestionGamePacket {
public:
	TwentyGameEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Game_End){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyEscapeQuizProvider : public TwentyQuestionGamePacket {
public:
	TwentyEscapeQuizProvider() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escpse_Quiz_Provider){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyEscapeAnotherPlayer : public TwentyQuestionGamePacket {
public:
	TwentyEscapeAnotherPlayer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escape_Another_Player){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyExitReservation : public TwentyQuestionGamePacket {
public:
	TwentyExitReservation() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Reservation){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyExitImmediately : public TwentyQuestionGamePacket {
public:
	TwentyExitImmediately() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Immediately){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyExitNotification : public TwentyQuestionGamePacket {
public:
	TwentyExitNotification() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Notification){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyRemainQuestion : public TwentyQuestionGamePacket {
public:
	TwentyRemainQuestion() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Remain_Question){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

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