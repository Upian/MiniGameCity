#pragma once
#ifndef _IN_GAME_MAIN_H_
#define _IN_GAME_MAIN_H_

#include <WinSock2.h>
#include <vector>

enum Game_Mode
{
	Twenty_Question,
	Relay_Novels_Writing,
	Ban_Word_Game,
	Catch_Mind
};
enum Twenty_Packet_Name
{
	//������
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
	//������
	Relay_Start_Response,
	Relay_Sentens_Request,
	Relay_Sentens_Response,
	Relay_Score_Request,
	Relay_Next_Response,
	Relay_Timer
};
enum Ban_Packet_Name
{
	//������
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
	//������
	Catch_Start_Response,
	Catch_Quiz_Answer_Response,
	Catch_Paint_Response,
	Catch_Paint_Request,
	Catch_Chat_Response,
	Catch_Chat_Request,
	Catch_Timer
};
struct simpleTypePacket; //�ٸ� ���� ���� ��Ŷ
struct intTypePacket; // int ���� �߰� ��Ŷ
struct charTypePacket; // char �迭 �߰� ��Ŷ
struct boolTypePacket; //bool ���� �߰� ��Ŷ

#endif