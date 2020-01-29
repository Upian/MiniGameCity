#include <WinSock2.h>
#include <time.h>

#include "Player.h"
#include "InGame.h"

InGame::InGame() {};
InGame::~InGame() {};

/*
1. 스무고개
2. 릴레이 소설
3. 금칙어 게임
4. 캐치 마인드
*/

void InGame::Twenty_Question_Game() {

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

	//플레이어 수만큼 라운드 진행을 위한 for문
	for (Game_Round = 1; Game_Round < Ingame_Player.size() + 1; ++Game_Round) {

		//첫 질문자 설정
		Questioner = Asker.begin();

		//질문 및 타이머를 반복하기 위한 while문
		while (1)
		{
			//recv 받을 시 switch문

			//timer 셋팅
		}

		//다음 출제자 지정
		++Quiz_Provider;
	}
}

void InGame::Next_Asker()
{
	++Questioner;
	if (Questioner->GetGPID() == Quiz_Provider->GetGPID())
	{
		++Questioner;
	}
}