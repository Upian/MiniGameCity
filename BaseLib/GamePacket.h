#pragma once
#ifndef _GAME_PACKET_H_
#define _GAME_PACKET_H_

enum Game_Mode
{
	Twenty,
	Relay,
	Ban,
	Catch
};

enum Start_Packet_Name
{
	Start_Master,
	Start_User,
	Start_Player_Location,
	Start_Player_Ready,
	Start_Server_Timer_Start,
	Start_Server_Timer_End
};
enum Twenty_Packet_Name
{
	Twenty_Quiz_Answer_Response,
	Twenty_Is_Questioner_Response,
	Twenty_Question_Request,
	Twenty_Question_Response,
	Twenty_Question_OX_Request,
	Twenty_Question_OX_Response,
	Twenty_Answer_Request,
	Twenty_Answer_O_Response,
	Twenty_Answer_X_Response,
	Twenty_Timer
};
enum Relay_Packet_Name
{
	Relay_Start_Response,
	Relay_Sentens_Request,
	Relay_Sentens_Response,
	Relay_Score_Request,
	Relay_Next_Response,
	Relay_Timer
};
enum Ban_Packet_Name
{
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
	Catch_Start_Response,
	Catch_Quiz_Answer_Response,
	Catch_Paint_Response,
	Catch_Paint_Request,
	Catch_Chat_Response,
	Catch_Chat_Request,
	Catch_Timer
};

#endif