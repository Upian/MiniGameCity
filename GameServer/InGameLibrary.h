#pragma once
#ifndef _IN_GAME_MAIN_H_
#define _IN_GAME_MAIN_H_

#include <vector>
#include "InGamePacket.h"
#include "PlayerManager.h"
#include "UTF8.h"

class InGameLibrary {

public:

	//게임시작 함수
	void TwentyQuestionGame(PlayerManager&);			//스무고개
	void RelayNovelGame(PlayerManager&) {}				//릴레이소설
	void BanKeywordGame(PlayerManager&) {}				//금칙어 게임
	void CatchMindGame(PlayerManager&) {}				//캐치마인드

	void HandleGameTwentyPacket();
private:

	//사용할 변수
	int			GameRound;				//게임 라운드
	int			QuestionCount;			//질문 횟수 남은 것
	int			PlayerCount;			//인게임 플레이어 수
	int			LoopCount;				//반복문 카운터
	int			AskerTimer;				//질문자 질문 타이머
	int			ProviderTimer;			//출제자 답변 타이머
	bool		AskerTurn;				//지금이 어떤 시간인지 구분하기위함.
	time_t		ActionTime;				//타이머 동작하는 시간
	Buffer*		RecvBuf;				//패킷작업시 패킷을 담을 공간.
	char		TwentyAnswer[31];		//스무고개 정답 변수

	//플레이어 정보를 담을 포인터
	PlayerManager InGamePlayerManager;
	std::list<std::shared_ptr<Player>> InGamePlayerList;
	std::vector<std::shared_ptr<Player>> InGamePlayer;	
	std::vector<std::shared_ptr<Player>> AskerGroup;

	//출제자와 질문자를 가리킬 iterator
	std::vector<std::shared_ptr<Player>>::iterator Asker;
	std::shared_ptr<Player> Quiz_Provide_Player;

	//키워드와 시작 문장을 저장하는 리스트
	std::vector<std::string> Words;
	std::vector<std::string> Sentens;

	//UTF 적용하기 위한 클래스
	Util::Conversion conversion;

	//스무고개 함수
	void Next_Asker_Point();						//다음 질문자를 가리킴
	void Connect_Check_In_Wait_time(int setTime);	//대기 시간동안 플레이어들의 연결이 끊겼는지 확인
	void Game_Setting_On();							//게임 시작하기 전에 출제자와 질문자 그룹을 생성하는 동작
	void SetPlayerPosition();						//플레이어의 정보가 담긴 벡터를 셔플
	void AllPlayerReadyCheck();						//모든 플레이어가 준비가 되었는지 확인하는 과정
	void LoadingTime();								//시간을 맞추기 위한 1초 버리기 동작
	void TimerErrorProcess();						//타이머 동작이 문제가 생겼을 경우 동작
/*	bool Packet*/
	TwentyProviderSelectAnswer SelectFiveAnswer(TwentyProviderSelectAnswer packet);		//5개의 문제를 선택


	std::queue<std::pair<Player, BasePacket>> m_Q;
	//기타 함수
	void Read_File();								//파일 읽는 함수
	void Random_Int();								//무작위 숫자 리턴
	void Score_Set();								//점수계산 및 출력
};
#endif