#include <WinSock2.h>
#include <time.h>

#include "Player.h"
#include "InGame.h"

InGame::InGame() {};
InGame::~InGame() {};

/*
1. ������
2. ������ �Ҽ�
3. ��Ģ�� ����
4. ĳġ ���ε�
*/

void InGame::Twenty_Question_Game() {

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

	//�÷��̾� ����ŭ ���� ������ ���� for��
	for (Game_Round = 1; Game_Round < Ingame_Player.size() + 1; ++Game_Round) {

		//ù ������ ����
		Questioner = Asker.begin();

		//���� �� Ÿ�̸Ӹ� �ݺ��ϱ� ���� while��
		while (1)
		{
			//recv ���� �� switch��

			//timer ����
		}

		//���� ������ ����
		++Quiz_Provider;
	}
}

void InGame::Next_Asker()
{
	++Questioner;
	if (Questioner->GetGPID() == Quiz_Provider->GetGPID())
	{
		++Questioner;
	}
}