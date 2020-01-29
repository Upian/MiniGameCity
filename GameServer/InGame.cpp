#include "InGameMain.h"

void IngameMain(LPVOID arg) {
	/*
	필요한 것
	플레이어 정보.
	방 게임 타입.

	*** 메인서버로의 메시지 전달은 어떻게 해야하는가? ***
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
1. 스무고개
2. 릴레이 소설
3. 금칙어 게임
4. 캐치 마인드
*/

void Twenty_Question_Game(void* arg) {
	//사용할 변수
	int		Game_Round;		//게임 라운드
	int		Question_Count;	//질문 횟수 남은 것
	int		player_count;	//인게임 플레이어 수
	int		Asker_Timer;	//질문자 질문 타이머
	int		Provider_Timer;	//출제자 답변 타이머
	int		Doing_Time;		//타이머 동작하는 시간
	//플레이어 정보를 담을 벡터(?)
	std::vector<int> ingame_player;
	//출제자와 질문자를 가리킬 iterator
	std::vector<int>::iterator Quiz_Provider;
	std::vector<int>::iterator Questioner;

	//라운드 전에 설정해야 할 부분

	//플레이어 수만큼 라운드 진행을 위한 for문

	//질문 및 타이머를 반복하기 위한 while문
}