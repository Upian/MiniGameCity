// #include <WinSock2.h>
// #include <time.h>
// 
// #include "InGame.h"
// 
// InGame::InGame() {};
// InGame::~InGame() {};
// 
// /*
// 1. ������
// 2. ������ �Ҽ�
// 3. ��Ģ�� ����
// 4. ĳġ ���ε�
// */
// 
// void InGame::Twenty_Question_Game(PlayerManager inGamePlayerManager) {
// 
// 	���� ���� �����ؾ� �� �κ�
// 	Doing_Time = time(NULL);
// 	Asker_Timer = 30;
// 	Provider_Timer = 10;
// 	InGame_Player_List = inGamePlayerManager.GetPlayerList();
// 	auto a = InGame_Player_List.begin();
// 	auto b = *a;
// 	
// 	Quiz_Provide_Player = InGame_Player_List.begin();
// 		
// 	//�����ڸ� ������ ����鸸�� ���� ����
// 	for (std::shared_ptr<Player> p : InGame_Player_List)
// 	{
// 		test.push_back(*p);
// 	}
// 
// 	delete &test;
// 
// 	//�÷��̾���� ������ �̿��Ͽ� ������ �ڸ���ġ�� ��
// 
// 	//�÷��̾� ���� �غ� ����� ���� while��
// 	while (1)
// 	{
// 
// 	}
// 
// 	//�÷��̾� ����ŭ ���� ������ ���� for��
// 	for (Game_Round = 1; Game_Round < Ingame_Player.size() + 1; ++Game_Round) {
// 
// 		//ù ������ ����
// 		Asker = Asker_Group.begin();
// 
// 		//���� �� Ÿ�̸Ӹ� �ݺ��ϱ� ���� while��
// 		while (1)
// 		{
// 			//recv ���� �� switch��
// 
// 			//timer ����
// 		}
// 
// 		//���� ������ ����
// 		++Quiz_Provide_Player;
// 	}
// }
// 
// ���� �����ڸ� ����Ŵ
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
// 	//��Ŷ �ۼ� �� ������ ����
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