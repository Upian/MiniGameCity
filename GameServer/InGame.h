#pragma once
#ifndef _IN_GAME_MAIN_H_
#define _IN_GAME_MAIN_H_

#include <vector>
#include "InGamePacket.h"
#include "PlayerManager.h"


class InGame;

struct simpleTypePacket; //�ٸ� ���� ���� ��Ŷ
struct intTypePacket; // int ���� �߰� ��Ŷ
struct charTypePacket; // char �迭 �߰� ��Ŷ
struct boolTypePacket; //bool ���� �߰� ��Ŷ

class InGame {

public:

	//����� ����
	int		Game_Round;				//���� ����
	int		Question_Count;			//���� Ƚ�� ���� ��
	int		Player_Count;			//�ΰ��� �÷��̾� ��
	int		Loop_Count;				//�ݺ��� ī����
	int		Asker_Timer;			//������ ���� Ÿ�̸�
	int		Provider_Timer;			//������ �亯 Ÿ�̸�
	time_t	Action_Time;			//Ÿ�̸� �����ϴ� �ð�

	//�÷��̾� ������ ���� ������
	std::list<std::shared_ptr<Player>> InGame_Player_List;
	std::vector<std::shared_ptr<Player>> InGame_Player;
	std::vector<std::shared_ptr<Player>> Asker_Group;

	//�����ڿ� �����ڸ� ����ų iterator
	std::vector<std::shared_ptr<Player>>::iterator Quiz_Provide_Player;
	std::vector<std::shared_ptr<Player>>::iterator Asker;

	//�⺻ �Լ�
	InGame();
	~InGame()
	{
	}

	//���ӽ��� �Լ�
	void Twenty_Question_Game(PlayerManager);		//������
	void Relay_Novel_Game(PlayerManager);			//�����̼Ҽ�
	void Ban_Keyword_Game(PlayerManager);			//��Ģ�� ����
	void Catch_Mind_Game(PlayerManager);			//ĳġ���ε�

	//������ �Լ�
	void Next_Asker_Point();						//���� �����ڸ� ����Ŵ
	void Next_Quiz_Provider_Point();				//���� �����ڸ� ����Ŵ
	void Set_Asker_Group();							//������ �׷��� ����
	void Connect_Check_In_Wait_time();				//��� �ð����� �÷��̾���� ������ ������� Ȯ��
	void Game_Setting_On();							//���� �����ϱ� ���� �����ڿ� ������ �׷��� �����ϴ� ����
	void Recive_Buffer_Process(char*);				//Ŭ���̾�Ʈ���� ���� ���� ó���ϴ� �κ�.
	void SelectFiveAnswer();
	void SetPlayerPosition();

	//��Ÿ �Լ�
	void Read_File();								//���� �д� �Լ�
	void Random_Int();								//������ ���� ����
	void Score_Set();								//������� �� ���
};
#endif