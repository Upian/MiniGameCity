// #include <WinSock2.h>
// #include <time.h>
// 
// #include "InGame.h"
// 
// InGame::InGame() {};
// InGame::~InGame() {};
// 
// /*
// 1. 스무고개
// 2. 릴레이 소설
// 3. 금칙어 게임
// 4. 캐치 마인드
// */
// 
// void InGame::Twenty_Question_Game(PlayerManager inGamePlayerManager) {
// 
// 	라운드 전에 설정해야 할 부분
// 	Doing_Time = time(NULL);
// 	Asker_Timer = 30;
// 	Provider_Timer = 10;
// 	InGame_Player_List = inGamePlayerManager.GetPlayerList();
// 	auto a = InGame_Player_List.begin();
// 	auto b = *a;
// 	
// 	Quiz_Provide_Player = InGame_Player_List.begin();
// 		
// 	//출제자를 제외한 사람들만의 벡터 만듬
// 	for (std::shared_ptr<Player> p : InGame_Player_List)
// 	{
// 		test.push_back(*p);
// 	}
// 
// 	delete &test;
// 
// 	//플레이어들의 정보를 이용하여 무작위 자리배치를 함
// 
// 	//플레이어 게임 준비 통신을 위한 while문
// 	while (1)
// 	{
// 
// 	}
// 
// 	//플레이어 수만큼 라운드 진행을 위한 for문
// 	for (Game_Round = 1; Game_Round < Ingame_Player.size() + 1; ++Game_Round) {
// 
// 		//첫 질문자 설정
// 		Asker = Asker_Group.begin();
// 
// 		//질문 및 타이머를 반복하기 위한 while문
// 		while (1)
// 		{
// 			//recv 받을 시 switch문
// 
// 			//timer 셋팅
// 		}
// 
// 		//다음 출제자 지정
// 		++Quiz_Provide_Player;
// 	}
// }
// 
// 다음 질문자를 가리킴
// void InGame::Next_Asker_Point()
// {
// 	++Asker;
// 	if (Asker == Asker_Group.end())
// 	{
// 		Asker = Asker_Group.begin();
// 	}
// }
// 
// void InGame::Next_Quiz_Provider_Point()
// {
// 	++Quiz_Provide_Player;
// 
// }
// 
// void InGame::Connect_Check_In_Wait_time()
// {
// 	//패킷 작성 후 데이터 대입
// }
// 
// void InGame::Game_Setting_On()
// {
// 	Quiz_Provide_Player = Ingame_Player.begin() + Game_Round;
// 
// 	for (Player p : Ingame_Player)
// 	{
// 		if (p.GetGPID() == Quiz_Provide_Player->GetGPID())
// 		{
// 			continue;
// 		}
// 		Asker_Group.push_back(p);
// 	}
// }
// 
// void InGame::Recive_Buffer_Process(char* buf)
// {
// 	if (nullptr == buf) return;
// 
// 	Twenty_Packet_Type packet_name = Twenty_None;
// 
// 
// }