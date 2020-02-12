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
		InGamePacketType InGameType;
		Twenty_Packet_Type CheckType;

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
		InGamePacketType InGameType;
		Twenty_Packet_Type packetType;

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

	int Remaintime = 0;

	virtual Buffer& Serialize() override {

		buffer << Remaintime;

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
	TwentyAskerQuestionBroadCast(std::string name, std::string question) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Question_BroadCast)
	{
		PlayerName = name;
		Question = question;
	}

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
	TwentyProviderReply() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Reply) {}

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
	TwentyProviderReplyBroadCast(std::string name,int Reply) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Reply_Broadcast)
	{
		PlayerName = name;
		ReplyOX = Reply;
	}

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

	std::string AskerAnswer;

	virtual Buffer& Serialize() override {

		buffer << AskerAnswer;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> AskerAnswer;

	}
};
class TwentyAskerAnswerBroadCast : public TwentyQuestionGamePacket {
public:
	TwentyAskerAnswerBroadCast() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Answer_Broadcast){}

	std::string AskerAnswer;
	std::string AskerName;
	int AnswerResult = 0;

	virtual Buffer& Serialize() override {
		
		buffer << AskerAnswer;
		buffer << AskerName;
		buffer << AnswerResult;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> AskerAnswer;
		buf >> AskerName;
		buf >> AnswerResult;
	}
};
class TwentyNoticeNextAsker : public TwentyQuestionGamePacket {
public:
	TwentyNoticeNextAsker(std::string name, int status) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Notice_Next_Asker)
	{
		PlayerName = name;
		NoticeStatus = status;
	}

	std::string PlayerName;
	int NoticeStatus = 0; //0은 질문 답변 듣고 넘김, 1은 질문자 타임오버, 2는 출제자 타임오버

	virtual Buffer& Serialize() override {
		buffer << PlayerName;
		buffer << NoticeStatus;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> PlayerName;
		buf >> NoticeStatus;
	}
};
class TwentyRoundEnd : public TwentyQuestionGamePacket {
public:
	TwentyRoundEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Round_End){}

	std::string Provider;
	std::string Asker;
	int Round = 0;

	virtual Buffer& Serialize() override {

		buffer << Provider;
		buffer << Asker;
		buffer << Round;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> Provider;
		buf >> Asker;
		buf >> Round;
	}
};
class TwentyUpdateScore : public TwentyQuestionGamePacket {
public:
	TwentyUpdateScore(std::string name, int update) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Update_Score)
	{
		PlayerName = name;
		Score = update;
	}

	std::string PlayerName;
	int Score;

	virtual Buffer& Serialize() override {
		buffer << PlayerName;
		buffer << Score;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> PlayerName;
		buf >> Score;
	}
};

class TwentyAnswerOpen : public TwentyQuestionGamePacket {
public:
	TwentyAnswerOpen() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Answer_Open){}

	std::string Answer;

	virtual Buffer& Serialize() override {
		buffer << Answer;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> Answer;
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

	std::string RankSortPlayer[5];
	int RankScore[5];

	virtual Buffer& Serialize() override {
		for (std::string s : RankSortPlayer)
		{
			buffer << s;
		}
		for (int i : RankScore)
		{
			buffer << i;
		}
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> RankSortPlayer[0];
		buf >> RankSortPlayer[1];
		buf >> RankSortPlayer[2];
		buf >> RankSortPlayer[3];
		buf >> RankSortPlayer[4];

		buf >> RankScore[0];
		buf >> RankScore[1];
		buf >> RankScore[2];
		buf >> RankScore[3];
		buf >> RankScore[4];
	}
};
class TwentyEscapeQuizProvider : public TwentyQuestionGamePacket {
public:
	TwentyEscapeQuizProvider() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escpse_Quiz_Provider){}

	std::string PlayerName;
	int RemainMember = 0;	//남은 멤버가 한명일 경우 1을 설정

	virtual Buffer& Serialize() override {

		buffer << PlayerName;
		buffer << RemainMember;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> PlayerName;
		buf >> RemainMember;
	}
};
class TwentyEscapeAnotherPlayer : public TwentyQuestionGamePacket {
public:
	TwentyEscapeAnotherPlayer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escape_Another_Player){}

	std::string PlayerName;
	int RemainMember = 0;	//남은 멤버가 한명일 경우 1을 설정
	int AskerOrWaiter = 0;	//질문자는 1, 대기자는 0

	virtual Buffer& Serialize() override {
		buffer << PlayerName;
		buffer << RemainMember;
		buffer << AskerOrWaiter;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> PlayerName;
		buf >> RemainMember;
		buf >> AskerOrWaiter;
	}
};
class TwentyExitReservation : public TwentyQuestionGamePacket {
public:
	TwentyExitReservation() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Reservation){}

	int ReservationType = 0; //0은 예약 취소, 1은 나가기 예약

	virtual Buffer& Serialize() override {
		buffer << ReservationType;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> ReservationType;
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

	std::string PlayerName;
	int ReservationType = 0;	//0은 예약 취소, 1은 나가기 예약

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyRemainQuestion : public TwentyQuestionGamePacket {
public:
	TwentyRemainQuestion(int count) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Remain_Question)
	{
		RemainQuestionCount = count;
	}

	int RemainQuestionCount;

	virtual Buffer& Serialize() override {
		buffer << RemainQuestionCount;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> RemainQuestionCount;
	}
};

class RelayNovelWritingGamePacket : public InGamePacket {

};
class BanKeywordGamePacket : public InGamePacket {

};
class CatchMianGamePacket : public InGamePacket {

};
#endif