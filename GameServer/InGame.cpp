#include "InGameMain.h"

void IngameMain(LPVOID arg) {
	/*
	�ʿ��� ��
	�÷��̾� ����.
	�� ���� Ÿ��.

	*** ���μ������� �޽��� ������ ��� �ؾ��ϴ°�? ***
	*/
	int check_game_mode;

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

void Twenty_Question_Game(void* arg) {
	//����� ����
	int		Game_Round;		//���� ����
	int		Question_Count;	//���� Ƚ�� ���� ��
	int		player_count;	//�ΰ��� �÷��̾� ��
	int		Asker_Timer;	//������ ���� Ÿ�̸�
	int		Provider_Timer;	//������ �亯 Ÿ�̸�
	int		Doing_Time;		//Ÿ�̸� �����ϴ� �ð�
	//�÷��̾� ������ ���� ����(?)
	std::vector<int> ingame_player;
	//�����ڿ� �����ڸ� ����ų iterator
	std::vector<int>::iterator Quiz_Provider;
	std::vector<int>::iterator Questioner;

	//���� ���� �����ؾ� �� �κ�

	//�÷��̾� ����ŭ ���� ������ ���� for��

	//���� �� Ÿ�̸Ӹ� �ݺ��ϱ� ���� while��
}