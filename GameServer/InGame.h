#pragma once
#ifndef _IN_GAME_MAIN_H_
#define _IN_GAME_MAIN_H_

#include <vector>

class InGame;

struct simpleTypePacket; //�ٸ� ���� ���� ��Ŷ
struct intTypePacket; // int ���� �߰� ��Ŷ
struct charTypePacket; // char �迭 �߰� ��Ŷ
struct boolTypePacket; //bool ���� �߰� ��Ŷ

class InGame {

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

	//�⺻ �Լ�
	InGame();
	~InGame();

	//���ӽ��� �Լ�
	void Twenty_Question_Game();	//������
	void Relay_Novel_Game();		//�����̼Ҽ�
	void Ban_Keyword_Game();		//��Ģ�� ����
	void Catch_Mind_Game();			//ĳġ���ε�

	//������ �Լ�
	void Next_Asker();				//���� �����ڸ� ����Ŵ

	//��Ÿ �Լ�
	void Read_File();				//���� �д� �Լ�
	void Random_Int();				//������ ���� ����
	void Score_Set();				//������� �� ���
};
#endif