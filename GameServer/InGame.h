#pragma once
#ifndef _IN_GAME_MAIN_H_
#define _IN_GAME_MAIN_H_

#include <vector>
#include "InGamePacket.h"
#include "PlayerManager.h"


class InGame;

struct simpleTypePacket; //다른 정보 없는 패킷
struct intTypePacket; // int 변수 추가 패킷
struct charTypePacket; // char 배열 추가 패킷
struct boolTypePacket; //bool 변수 추가 패킷

class InGame {

public:

	//게임시작 함수
	void TwentyQuestionGame(PlayerManager&);			//스무고개
	void RelayNovelGame(PlayerManager&);			//릴레이소설
	void BanKeywordGame(PlayerManager&);			//금칙어 게임
	void CatchMindGame(PlayerManager&);			//캐치마인드

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
	BufferInfo*	RecvBuf;				//패킷작업시 패킷을 담을 공간.
	std::string	TwentyAnswer;			//스무고개 정답 변수

	//플레이어 정보를 담을 포인터
	std::list<std::shared_ptr<Player>> InGamePlayerList;
	std::vector<std::shared_ptr<Player>> InGamePlayer;
	std::vector<std::shared_ptr<Player>> AskerGroup;

	//출제자와 질문자를 가리킬 iterator
	std::vector<std::shared_ptr<Player>>::iterator Quiz_Provide_Player;
	std::vector<std::shared_ptr<Player>>::iterator Asker;

	//키워드와 시작 문장을 저장하는 리스트
	std::vector<std::string> Words;
	std::vector<std::string> Sentens;

	//기본 함수
	InGame();
	~InGame()
	{
	}

	

	//스무고개 함수
	void Next_Asker_Point();						//다음 질문자를 가리킴
	void Connect_Check_In_Wait_time();				//대기 시간동안 플레이어들의 연결이 끊겼는지 확인
	void Game_Setting_On();							//게임 시작하기 전에 출제자와 질문자 그룹을 생성하는 동작
	void SetPlayerPosition();						//플레이어의 정보가 담긴 벡터를 셔플
	void AllPlayerReadyCheck();						//모든 플레이어가 준비가 되었는지 확인하는 과정
	void LoadingTime();
	void TimerErrorProcess();
	TwentyProviderSelectAnswer SelectFiveAnswer(TwentyProviderSelectAnswer packet);

	//기타 함수
	void Read_File();								//파일 읽는 함수
	void Random_Int();								//무작위 숫자 리턴
	void Score_Set();								//점수계산 및 출력
};
#endif