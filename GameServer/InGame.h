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

	//사용할 변수
	int		Game_Round;			//게임 라운드
	int		Question_Count;		//질문 횟수 남은 것
	int		Player_Count;		//인게임 플레이어 수
	int		Loop_Count;			//반복문 카운터
	int		Asker_Timer;		//질문자 질문 타이머
	int		Provider_Timer;		//출제자 답변 타이머
	time_t	Doing_Time;			//타이머 동작하는 시간

	//플레이어 정보를 담을 포인터
	std::list<std::shared_ptr<Player>> InGame_Player_List;
	std::list<std::shared_ptr<Player>> Asker_Group;
	std::vector<Player> test;

	//출제자와 질문자를 가리킬 iterator
	std::list<std::shared_ptr<Player>>::iterator Quiz_Provide_Player;
	std::list<Player>::iterator Asker;

	//기본 함수
	InGame();
	~InGame();

	//게임시작 함수
	void Twenty_Question_Game(PlayerManager inGamePlayerManager);					//스무고개
	void Relay_Novel_Game(PlayerManager inGamePlayerManager);						//릴레이소설
	void Ban_Keyword_Game(PlayerManager inGamePlayerManager);						//금칙어 게임
	void Catch_Mind_Game(PlayerManager inGamePlayerManager);						//캐치마인드

	//스무고개 함수
	void Next_Asker_Point();						//다음 질문자를 가리킴
	void Next_Quiz_Provider_Point();				//다음 출제자를 가리킴
	void Set_Asker_Group();							//질문자 그룹을 만듬
	void Connect_Check_In_Wait_time();				//대기 시간동안 플레이어들의 연결이 끊겼는지 확인
	void Game_Setting_On();							//게임 시작하기 전에 출제자와 질문자 그룹을 생성하는 동작
	void Recive_Buffer_Process(char* buf);			//클라이언트에게 받은 값을 처리하는 부분.

	//기타 함수
	void Read_File();								//파일 읽는 함수
	void Random_Int();								//무작위 숫자 리턴
	void Score_Set();								//점수계산 및 출력
};
#endif