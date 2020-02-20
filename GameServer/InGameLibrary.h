#pragma once
#ifndef _IN_GAME_MAIN_H_
#define _IN_GAME_MAIN_H_

#include <vector>
#include "InGamePacket.h"
#include "PlayerManager.h"
#include "UTF8.h"

class InGameLibrary {

public:

	//���ӽ��� �Լ�
	void TwentyQuestionGame(PlayerManager&);			//������
	void RelayNovelGame(PlayerManager&) {}				//�����̼Ҽ�
	void BanKeywordGame(PlayerManager&) {}				//��Ģ�� ����
	void CatchMindGame(PlayerManager&) {}				//ĳġ���ε�

	void HandleGameTwentyPacket();
private:

	//����� ����
	int			GameRound;				//���� ����
	int			QuestionCount;			//���� Ƚ�� ���� ��
	int			PlayerCount;			//�ΰ��� �÷��̾� ��
	int			LoopCount;				//�ݺ��� ī����
	int			AskerTimer;				//������ ���� Ÿ�̸�
	int			ProviderTimer;			//������ �亯 Ÿ�̸�
	bool		AskerTurn;				//������ � �ð����� �����ϱ�����.
	time_t		ActionTime;				//Ÿ�̸� �����ϴ� �ð�
	Buffer*		RecvBuf;				//��Ŷ�۾��� ��Ŷ�� ���� ����.
	char		TwentyAnswer[31];		//������ ���� ����

	//�÷��̾� ������ ���� ������
	PlayerManager InGamePlayerManager;
	std::list<std::shared_ptr<Player>> InGamePlayerList;
	std::vector<std::shared_ptr<Player>> InGamePlayer;	
	std::vector<std::shared_ptr<Player>> AskerGroup;

	//�����ڿ� �����ڸ� ����ų iterator
	std::vector<std::shared_ptr<Player>>::iterator Asker;
	std::shared_ptr<Player> Quiz_Provide_Player;

	//Ű����� ���� ������ �����ϴ� ����Ʈ
	std::vector<std::string> Words;
	std::vector<std::string> Sentens;

	//UTF �����ϱ� ���� Ŭ����
	Util::Conversion conversion;

	//������ �Լ�
	void Next_Asker_Point();						//���� �����ڸ� ����Ŵ
	void Connect_Check_In_Wait_time(int setTime);	//��� �ð����� �÷��̾���� ������ ������� Ȯ��
	void Game_Setting_On();							//���� �����ϱ� ���� �����ڿ� ������ �׷��� �����ϴ� ����
	void SetPlayerPosition();						//�÷��̾��� ������ ��� ���͸� ����
	void AllPlayerReadyCheck();						//��� �÷��̾ �غ� �Ǿ����� Ȯ���ϴ� ����
	void LoadingTime();								//�ð��� ���߱� ���� 1�� ������ ����
	void TimerErrorProcess();						//Ÿ�̸� ������ ������ ������ ��� ����
/*	bool Packet*/
	TwentyProviderSelectAnswer SelectFiveAnswer(TwentyProviderSelectAnswer packet);		//5���� ������ ����


	std::queue<std::pair<Player, BasePacket>> m_Q;
	//��Ÿ �Լ�
	void Read_File();								//���� �д� �Լ�
	void Random_Int();								//������ ���� ����
	void Score_Set();								//������� �� ���
};
#endif