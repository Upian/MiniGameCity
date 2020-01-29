#pragma once
#ifndef _IN_GAME_MAIN_H_
#define _IN_GAME_MAIN_H_

#include <vector>

class InGame;

struct simpleTypePacket; //다른 정보 없는 패킷
struct intTypePacket; // int 변수 추가 패킷
struct charTypePacket; // char 배열 추가 패킷
struct boolTypePacket; //bool 변수 추가 패킷

class InGame {

	//사용할 변수
	int		Game_Round;			//게임 라운드
	int		Question_Count;		//질문 횟수 남은 것
	int		player_count;		//인게임 플레이어 수
	int		Asker_Timer;		//질문자 질문 타이머
	int		Provider_Timer;		//출제자 답변 타이머
	time_t	Doing_Time;			//타이머 동작하는 시간

	//플레이어 정보를 담을 벡터
	std::vector<Player> Ingame_Player;
	std::vector<Player> Asker;

	//출제자와 질문자를 가리킬 iterator
	std::vector<Player>::iterator Quiz_Provider;
	std::vector<Player>::iterator Questioner;

	//기본 함수
	InGame();
	~InGame();

	//게임시작 함수
	void Twenty_Question_Game();	//스무고개
	void Relay_Novel_Game();		//릴레이소설
	void Ban_Keyword_Game();		//금칙어 게임
	void Catch_Mind_Game();			//캐치마인드

	//스무고개 함수
	void Next_Asker();				//다음 질문자를 가리킴

	//기타 함수
	void Read_File();				//파일 읽는 함수
	void Random_Int();				//무작위 숫자 리턴
	void Score_Set();				//점수계산 및 출력
};
#endif