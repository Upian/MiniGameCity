#include "InGame.h"

InGame::InGame() {};
InGame::~InGame() {};

void InGame::Game_Start(LPVOID arg) {
	/*
	�ʿ��� ��
	�÷��̾� ����.
	�� ���� Ÿ��.

	*** ���μ������� �޽��� ������ ��� �ؾ��ϴ°�? ***
	*/
	Game_Mode check_game_mode = Game_Mode_None ;

	switch (check_game_mode) {
	case Twenty_Question:
		break;
	case Relay_Novels_Writing:
		break;
	case Ban_Word_Game:
		break;
	case Catch_Mind:
		break;
	}
}

/*
1. ������
2. ������ �Ҽ�
3. ��Ģ�� ����
4. ĳġ ���ε�
*/

void Twenty_Question_Game() {

	//����� ����
	int		Game_Round;			//���� ����
	int		Question_Count;		//���� Ƚ�� ���� ��
	int		player_count;		//�ΰ��� �÷��̾� ��
	int		Asker_Timer;		//������ ���� Ÿ�̸�
	int		Provider_Timer;		//������ �亯 Ÿ�̸�
	time_t	Doing_Time;			//Ÿ�̸� �����ϴ� �ð�

	//�÷��̾� ������ ���� ����
	std::vector<Player> Ingame_Player;
	std::vector<Player> Asker;

	//�����ڿ� �����ڸ� ����ų iterator
	std::vector<Player>::iterator Quiz_Provider;
	std::vector<Player>::iterator Questioner;

	//���� ���� �����ؾ� �� �κ�
	Doing_Time = time(NULL) + 1;
	Asker_Timer = 30;
	Provider_Timer = 10;
	Quiz_Provider = Ingame_Player.begin();
	
	//�����ڸ� ������ ����鸸�� ���� ����
	for (Player p : Ingame_Player)
	{
		if(p.GetGPID() == Quiz_Provider->GetGPID()) continue;
		Asker.push_back(p);
	}

	Questioner = Asker.begin();

	//�÷��̾� ����ŭ ���� ������ ���� for��
	for (Game_Round = 1; Game_Round < Ingame_Player.size() + 1; ++Game_Round) {

		//���� �� Ÿ�̸Ӹ� �ݺ��ϱ� ���� while��
		while (1)
		{
			//recv ���� �� switch��

			//timer ����
		}
	}
}