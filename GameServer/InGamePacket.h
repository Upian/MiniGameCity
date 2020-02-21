#pragma once
#ifndef _IN_GAME_PACKET_H_
#define _IN_GAME_PACKET_H_

#include <string>

#include "BasePacket.h"
#include "Player.h"

enum class InGamePacketType : char
{
	InGame_Packet_None,
	Twenty_Question_Game,
	Relay_Novels_Writing_Game,
	Ban_Keyword_Game,
	Catch_Mind_Game
};

enum class Twenty_Packet_Type : char
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
enum class Relay_Packet_Type : char
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
enum class Ban_Packet_Type : char
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
enum class Catch_Pakcet_Type : char
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
	InGamePacketType GetInGamePacketType() { return ingame_packet_type; }
	void SetInGamePacketType(InGamePacketType _ingame_packet_type) { ingame_packet_type = _ingame_packet_type; }
protected:
	InGamePacketType ingame_packet_type = InGamePacketType::InGame_Packet_None;
};

#pragma region Twenty
//Twenty Base Packet
class TwentyQuestionGamePacket : public InGamePacket {
public:
	TwentyQuestionGamePacket(Twenty_Packet_Type _twenty_packet) : InGamePacket(InGamePacketType::Twenty_Question_Game), twenty_packet(_twenty_packet)
	{
		this->PacketTypeSerial(_twenty_packet);
	}
	Twenty_Packet_Type getTwentyPacket() { return twenty_packet; }
protected:
	Twenty_Packet_Type twenty_packet = Twenty_Packet_Type::Twenty_None;
};

class TwentyPlayerLocationSettingPacket : public TwentyQuestionGamePacket {
public:
	TwentyPlayerLocationSettingPacket() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Player_Location_Setting) {}

	char PlayerName[5][31] = { NULL, };
	int PlayerCount = 0;

	virtual Buffer& Serialize() override {

		for (int i = 0; i < 5; ++i) {
			buffer.SerializeCharArray(PlayerName[i], 31);
		}
		buffer << PlayerCount;

		return buffer;
	}

	virtual void Deserialize(Buffer& _buf) override {

	}
private:

};
class TwentyPlayerReadyCompletePacket : public TwentyQuestionGamePacket {
public:
	TwentyPlayerReadyCompletePacket() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_None){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> twenty_packet;
	}
};
class TwentyCountDownStart : public TwentyQuestionGamePacket {
public:
	TwentyCountDownStart() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart_Timer_On) {}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> ingame_packet_type;
		buf >> twenty_packet;
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
		buf >> ingame_packet_type;
		buf >> twenty_packet;
	}
};
class TwentyProviderSelectAnswer : public TwentyQuestionGamePacket
{
public:
	TwentyProviderSelectAnswer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provide_5Answer){}

	char AnswerChoice[5][31] = { NULL, };

	virtual Buffer& Serialize() override {
		
		for (int i = 0; i < 5; ++i)
		{
			buffer.SerializeCharArray(AnswerChoice[i], 31);
		}
		
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentySelectAnswer : public TwentyQuestionGamePacket {
public:
	TwentySelectAnswer() :TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_None){}


	char Answer[31] = { NULL, };

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> twenty_packet;
		buf.DeserializeCharArray(Answer, 31);
	}
};
class TwentyGameStart :public TwentyQuestionGamePacket {
public:
	TwentyGameStart() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_GameStart){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> ingame_packet_type;
		buf >> twenty_packet;
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
		buf >> ingame_packet_type;
		buf >> twenty_packet;
		buf >> Remaintime;
	}
};
class TwentyAskerQuestion : public TwentyQuestionGamePacket {
public:
	TwentyAskerQuestion() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_None) {}

	char Question[91] = { NULL, };

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		
		buf.DeserializeCharArray(Question, 91);
	}
};
class TwentyAskerQuestionBroadCast : public TwentyQuestionGamePacket {
public:
	TwentyAskerQuestionBroadCast(char* name, char* question) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Question_BroadCast)
	{
		strcpy_s(PlayerName, 31, name);
		strcpy_s(Question, 91, question);
	}

	char PlayerName[31] = { NULL, };
	char Question[91] = { NULL, };

	virtual Buffer& Serialize() override {

		buffer.SerializeCharArray(PlayerName, 31);
		buffer.SerializeCharArray(Question, 91);

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyProviderReply : public TwentyQuestionGamePacket {
public:
	TwentyProviderReply() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_None) {}

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
	TwentyProviderReplyBroadCast(char* name,int Reply) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Provider_Reply_Broadcast)
	{
		strcpy_s(PlayerName, 31, name);
		ReplyOX = Reply;
	}

	char PlayerName[31] = { NULL, };
	int ReplyOX = 0;

	virtual Buffer& Serialize() override {

		buffer.SerializeCharArray(PlayerName, 31);
		buffer << ReplyOX;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyAskerAnswer : public TwentyQuestionGamePacket {
public:
	TwentyAskerAnswer() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_None){}

	char AskerAnswer[31] = { NULL, };

	virtual Buffer& Serialize() override {

		buffer.SerializeCharArray(AskerAnswer, 31);

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf.DeserializeCharArray(AskerAnswer, 31);
	}
};
class TwentyAskerAnswerBroadCast : public TwentyQuestionGamePacket {
public:
	TwentyAskerAnswerBroadCast(char* answer, char* name , int answervalue) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Asker_Answer_Broadcast)
	{
		strcpy_s(AskerAnswer, 31, answer);
		strcpy_s(AskerName, 31, name);
		AnswerResult = answervalue;
	}

	char AskerAnswer[31] = { NULL, };
	char AskerName[31] = { NULL, };
	int AnswerResult = 0;

	virtual Buffer& Serialize() override {
		
		buffer.SerializeCharArray(AskerAnswer, 31);
		buffer.SerializeCharArray(AskerName, 31);
		buffer << AnswerResult;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		
	}
};
class TwentyNoticeNextAsker : public TwentyQuestionGamePacket {
public:
	TwentyNoticeNextAsker(char* name, int status) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Notice_Next_Asker)
	{
		strcpy_s(PlayerName, 31, name);
		NoticeStatus = status;
	}

	char PlayerName[31] = { NULL, };
	int NoticeStatus = 0; //0은 질문 답변 듣고 넘김, 1은 질문자 타임오버, 2는 출제자 타임오버

	virtual Buffer& Serialize() override {

		buffer.SerializeCharArray(PlayerName, 31);
		buffer << NoticeStatus;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyRoundEnd : public TwentyQuestionGamePacket {
public:
	TwentyRoundEnd(char* prov, char* ask, int round) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Round_End)
	{
		strcpy_s(Provider, 31, prov);
		strcpy_s(Asker, 31, ask);
		Round = round;
	}

	char Provider[31] = { NULL, };
	char Asker[31] = { NULL };
	int Round = 0;

	virtual Buffer& Serialize() override {

		buffer.SerializeCharArray(Provider, 31);
		buffer.SerializeCharArray(Asker, 31);
		buffer << Round;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		
		buf >> ingame_packet_type;
		buf >> twenty_packet;

	}
};
class TwentyUpdateScore : public TwentyQuestionGamePacket {
public:
	TwentyUpdateScore(char* name, int update) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Update_Score)
	{
		strcpy_s(PlayerName, 31, name);
		Score = update;
	}

	char PlayerName[31] = { NULL,};
	int Score = 0;

	virtual Buffer& Serialize() override {
		buffer.SerializeCharArray(PlayerName, 31);
		buffer << Score;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> ingame_packet_type;
		buf >> twenty_packet;
	}
};

class TwentyAnswerOpen : public TwentyQuestionGamePacket {
public:
	TwentyAnswerOpen() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Answer_Open){}

	char Answer[31] = { NULL, };

	virtual Buffer& Serialize() override {

		buffer.SerializeCharArray(Answer, 31);

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> ingame_packet_type;
		buf >> twenty_packet;
	}
};
class TwentyRestTime : public TwentyQuestionGamePacket {
public:
	TwentyRestTime() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Rest_Time){}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> ingame_packet_type;
		buf >> twenty_packet;
	}
};
class TwentyGameEnd : public TwentyQuestionGamePacket {
public:
	TwentyGameEnd() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Game_End){}

	char RankSortPlayer[5][31] = { NULL, };
	int RankScore[5] = { NULL, };

	virtual Buffer& Serialize() override {
		for (int i = 0; i < 5; ++i)
		{
			buffer.SerializeCharArray(RankSortPlayer[i], 31);
		}
		for (int i : RankScore)
		{
			buffer << i;
		}
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
	}
};
class TwentyEscapeQuizProvider : public TwentyQuestionGamePacket {
public:
	TwentyEscapeQuizProvider(char* name) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escpse_Quiz_Provider)
	{
		strcpy_s(PlayerName, 31, name);
	}

	char PlayerName[31] = { NULL, };
	int RemainMember = 0;	//남은 멤버가 한명일 경우 1을 설정

	virtual Buffer& Serialize() override {

		buffer.SerializeCharArray(PlayerName, 31);
		buffer << RemainMember;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
	}
};
class TwentyEscapeAnotherPlayer : public TwentyQuestionGamePacket {
public:
	TwentyEscapeAnotherPlayer(char* name) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Escape_Another_Player)
	{
		strcpy_s(PlayerName, 31 ,name);
	}

	char PlayerName[31] = { NULL, };
	int RemainMember = 0;	//남은 멤버가 한명일 경우 1을 설정
	int AskerOrWaiter = 0;	//질문자는 1, 대기자는 0

	virtual Buffer& Serialize() override {
		buffer.SerializeCharArray(PlayerName, 31);
		buffer << RemainMember;
		buffer << AskerOrWaiter;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

	}
};
class TwentyExitReservation : public TwentyQuestionGamePacket {
public:
	TwentyExitReservation() : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_None){}

	int ReservationType = 0; //0은 예약 취소, 1은 나가기 예약

	virtual Buffer& Serialize() override {
		buffer << ReservationType;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> twenty_packet;
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
		buf >> twenty_packet;
	}
};
class TwentyExitNotification : public TwentyQuestionGamePacket {
public:
	TwentyExitNotification(char* name,int type) : TwentyQuestionGamePacket(Twenty_Packet_Type::Twenty_Exit_Notification)
	{
		strcpy_s(PlayerName, 31, name);
		ReservationType = type;
	}

	char PlayerName[31] = { NULL, };
	int ReservationType = 0;	//0은 예약 취소, 1은 나가기 예약

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> twenty_packet;
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
		buf >> ingame_packet_type;
		buf >> twenty_packet;
		buf >> RemainQuestionCount;
	}
};
#pragma endregion Twenty

#pragma region Relay
class RelayNovelWritingGamePacket : public InGamePacket {

};

#pragma endregion Relay 

class BanKeywordGamePacket : public InGamePacket {

};
class CatchMianGamePacket : public InGamePacket {

};
#endif