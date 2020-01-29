#include "InGame.h"

InGame::InGame() {};
InGame::~InGame() {};

void InGame::Game_Start(LPVOID arg) {
	/*
	필요한 것
	플레이어 정보.
	방 게임 타입.

	*** 메인서버로의 메시지 전달은 어떻게 해야하는가? ***
	*/
	Game_Mode check_game_mode = Game_Mode_None ;

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

void Twenty_Question_Game() {

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

	//라운드 전에 설정해야 할 부분
	Doing_Time = time(NULL) + 1;
	Asker_Timer = 30;
	Provider_Timer = 10;
	Quiz_Provider = Ingame_Player.begin();
	
	//출제자를 제외한 사람들만의 벡터 만듬
	for (Player p : Ingame_Player)
	{
		if(p.GetGPID() == Quiz_Provider->GetGPID()) continue;
		Asker.push_back(p);
	}

	Questioner = Asker.begin();

	//플레이어 수만큼 라운드 진행을 위한 for문
	for (Game_Round = 1; Game_Round < Ingame_Player.size() + 1; ++Game_Round) {

		//질문 및 타이머를 반복하기 위한 while문
		while (1)
		{
			//recv 받을 시 switch문

			//timer 셋팅
		}
	}
}