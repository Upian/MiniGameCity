#include <WinSock2.h>
#include <time.h>
#include <random>
#include <algorithm>

#include "InGame.h"

/*
1. 스무고개
2. 릴레이 소설
3. 금칙어 게임
4. 캐치 마인드
*/

bool CompareScore(const std::shared_ptr<Player> x, const std::shared_ptr<Player> y)
{
	return x->GetGameScore() > y->GetGameScore();
}

void InGame::TwentyQuestionGame(PlayerManager& _InGamePlayerManager) {

	//게임 준비 파트 변수 생성
	this->InGamePlayerManager = _InGamePlayerManager;
	InGamePlayerList = InGamePlayerManager.GetPlayerList();
	for (auto player : InGamePlayerList)
	{
		InGamePlayer.push_back(player);
	}
	for (auto player : InGamePlayerList)
	{
		PlayerNameList.push_back(player->GetPlayerName());
	}
	InGamePlayer.reserve(5);
	AskerGroup.reserve(4);

	//플레이어들의 정보를 이용하여 무작위 자리배치를 함
	SetPlayerPosition();

	TwentyPlayerLocationSettingPacket packet;
	for (int i = 0; i < InGamePlayer.size();++i)
	{
		packet.PlayerName[i] = InGamePlayer[i]->GetPlayerName();
	}
	packet.PlayerCount = InGamePlayer.size();

	InGamePlayerManager.SendToAllPlayers(packet);

	//플레이어 수만큼 라운드 진행을 위한 for문
	for (GameRound = 0; GameRound < InGamePlayer.size();) {

		//플레이어 게임 준비 통신을 위한 while문
		while (1)
		{
			AllPlayerReadyCheck();
			LoadingTime();
			TwentyCountDownStart startpacket;
			InGamePlayerManager.SendToAllPlayers(startpacket);

			//연결이 끊어진 사람이 존재하는가 판단해야 하는 부분(현재 미구현) - 3초
			Sleep(3000);
			Game_Setting_On();

			TwentyProviderSelectAnswer selectpacket;
			selectpacket = SelectFiveAnswer(selectpacket);
			(Quiz_Provide_Player)->SendPacket(selectpacket);

			TwentyCountDownEnd endpacket;
			InGamePlayerManager.SendToAllPlayers(endpacket);

			//연결이 끊어진 사람이 존재하는가 판단해야 하는 부분(현재 미구현) - 10초
			Sleep(10000);
			TwentyGameStart GameStartPacket;
			InGamePlayerManager.SendToAllPlayers(GameStartPacket);

			while (1)
			{
				TwentySelectAnswer SelectedAnswer;
				RecvBuf = nullptr;
				RecvBuf = (Quiz_Provide_Player)->GetGamePacket();
				if (RecvBuf != nullptr)
				{
					SelectedAnswer.Deserialize(RecvBuf->buffer);
					TwentyAnswer = SelectedAnswer.Answer;
					break;
				}
			}
		}

		//인게임 내에서 사용할 변수 셋팅
		QuestionCount = 20;
		AskerTimer = 30;
		ProviderTimer = 15;
		AskerTurn = true;
		ActionTime = time(NULL);

		//질문 및 타이머를 반복하기 위한 while문
		while (1)
		{
			for (int i = 0; i < InGamePlayer.size(); ++i)
			{
				RecvBuf = InGamePlayer[i]->GetGamePacket();
				if (RecvBuf == nullptr) break;

				// 받은 패킷 세분화 과정
				InGamePacketType GameType;
				GameType = (InGamePacketType)PacketTypeDeserial(RecvBuf->buffer);

				if (GameType != InGamePacketType::Twenty_Question_Game) break;

				Twenty_Packet_Type PacketType;
				PacketType = (Twenty_Packet_Type)PacketTypeDeserial(RecvBuf->buffer);

				switch (PacketType)
				{
				case Twenty_Packet_Type::Twenty_Asker_Question:
				{
					if ((*Asker)->GetGPID() != InGamePlayer[i]->GetGPID()) break;

					//받은 질문을 다시 뿌려줌
					TwentyAskerQuestion QuestionPacket;
					QuestionPacket.Deserialize(RecvBuf->buffer);
					TwentyAskerQuestionBroadCast QuestionBroadCastPacket(InGamePlayer[i]->GetPlayerName(), QuestionPacket.Question);
					InGamePlayerManager.SendToAllPlayers(QuestionBroadCastPacket);

					//남은 질문횟수 뿌려줌
					QuestionCount -= 1;
					TwentyRemainQuestion RemainCountPacket(QuestionCount);
					InGamePlayerManager.SendToAllPlayers(RemainCountPacket);

					AskerTimer = 30;
					AskerTurn = false;
					ActionTime = time(NULL);
					break;
				}
				case Twenty_Packet_Type::Twenty_Provider_Reply:
				{
					if (Quiz_Provide_Player->GetGPID() != InGamePlayer[i]->GetGPID()) break;

					//받은 답변을 뿌려줌
					TwentyProviderReply ReplyPacket;
					ReplyPacket.Deserialize(RecvBuf->buffer);
					TwentyProviderReplyBroadCast ReplyBroadcastPacket(InGamePlayer[i]->GetPlayerName(), ReplyPacket.ReplyOX);
					InGamePlayerManager.SendToAllPlayers(ReplyBroadcastPacket);

					if (QuestionCount != 0)
					{
						Next_Asker_Point();

						//다음 질문자를 알려줌
						TwentyNoticeNextAsker NoticeAskerPacket((*Asker)->GetPlayerName(), 0);
						InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);

						ProviderTimer = 15;
						AskerTurn = true;
						ActionTime = time(NULL);
					}
					else {
						TwentyAnswerOpen AnswerPacket;
						AnswerPacket.Answer = TwentyAnswer;
						InGamePlayerManager.SendToAllPlayers(AnswerPacket);
					}

					break;
				}
				case Twenty_Packet_Type::Twenty_Asker_Answer:
				{
					if ((*Asker)->GetGPID() != InGamePlayer[i]->GetGPID()) break;

					TwentyAskerAnswer AnswerPacket;
					AnswerPacket.Deserialize(RecvBuf->buffer);

					TwentyAskerAnswerBroadCast AnswerBroadCastPacket(InGamePlayer[i]->GetPlayerName(),AnswerPacket.AskerAnswer,0);

					if (AnswerPacket.AskerAnswer == TwentyAnswer)
					{
						//정답을 맞췄을 경우 동작
						AnswerBroadCastPacket.AnswerResult = 1;
						InGamePlayerManager.SendToAllPlayers(AnswerBroadCastPacket);
						
						(Quiz_Provide_Player)->UpdateGameScore(QuestionCount * 2);
						(*Asker)->UpdateGameScore(QuestionCount * 5);

						TwentyUpdateScore providerScorePacket((Quiz_Provide_Player)->GetPlayerName(),(Quiz_Provide_Player)->GetGameScore());
						TwentyUpdateScore askerScorerPacker((*Asker)->GetPlayerName(), (*Asker)->GetGameScore());

						InGamePlayerManager.SendToAllPlayers(providerScorePacket);
						InGamePlayerManager.SendToAllPlayers(askerScorerPacker);

						ActionTime = 0;
						QuestionCount = 0;
					}
					else
					{
						 //정답을 틀렸을 경우 동작
						AnswerBroadCastPacket.AnswerResult = 0;
						InGamePlayerManager.SendToAllPlayers(AnswerBroadCastPacket);

						QuestionCount -= 1;

						TwentyRemainQuestion RemainCountPacket(QuestionCount);
						InGamePlayerManager.SendToAllPlayers(RemainCountPacket);

						if (QuestionCount != 0) {
							Next_Asker_Point();
							TwentyNoticeNextAsker NoticeAskerPacket(InGamePlayer[i]->GetPlayerName(), 0);
							InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);

							AskerTimer = 30;
							ActionTime = time(NULL);
						}
						else
						{
							TwentyAnswerOpen AnswerPacket;
							AnswerPacket.Answer = TwentyAnswer;
							InGamePlayerManager.SendToAllPlayers(AnswerPacket);
						}
					}

					break;
				}
				case Twenty_Packet_Type::Twenty_Exit_Reservation:
				{
					TwentyExitReservation exitReservationPacket;
					exitReservationPacket.Deserialize(RecvBuf->buffer);
					InGamePlayer[i]->SetExitReservation(exitReservationPacket.ReservationType);
					TwentyExitNotification exitNotificationPacket(InGamePlayer[i]->GetPlayerName(),exitReservationPacket.ReservationType);

					break;
				}
				default:
				{
					break;
				}
				}
			}

			//timer 셋팅
			TimerErrorProcess();

			if (ActionTime == time(NULL))
			{
				++ActionTime;
				TwentyInGameTimer InGameTimerPacket;
				if (AskerTurn)
				{
					InGameTimerPacket.Remaintime = AskerTimer;
					InGamePlayerManager.SendToAllPlayers(InGameTimerPacket);
					if (AskerTimer <= 0)
					{
						AskerTimer = 31;
						QuestionCount -= 1;
						TwentyRemainQuestion RemainCountPacket(QuestionCount);
						InGamePlayerManager.SendToAllPlayers(RemainCountPacket);

						(*Asker)->UpdateGameScore(-5);
						TwentyUpdateScore AskerScorePacketr((*Asker)->GetPlayerName(),(*Asker)->GetGameScore());
						InGamePlayerManager.SendToAllPlayers(AskerScorePacketr);

						if (QuestionCount != 0)
						{
							Next_Asker_Point();
							TwentyNoticeNextAsker NoticeAskerPacket((*Asker)->GetPlayerName(), 1);
							InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);

							ActionTime = time(NULL);
						}
						else {
							TwentyAnswerOpen AnswerPacket;
							AnswerPacket.Answer = TwentyAnswer;
							InGamePlayerManager.SendToAllPlayers(AnswerPacket);
						}
					}
					--AskerTimer;
				}
				else 
				{
					InGameTimerPacket.Remaintime = ProviderTimer;
					InGamePlayerManager.SendToAllPlayers(InGameTimerPacket);

					if (ProviderTimer <= 0)
					{
						(Quiz_Provide_Player)->UpdateGameScore(-15);
						TwentyUpdateScore ProviderScorePacket((Quiz_Provide_Player)->GetPlayerName(),(Quiz_Provide_Player)->GetGameScore());
						InGamePlayerManager.SendToAllPlayers(ProviderScorePacket);

						Next_Asker_Point();
						TwentyNoticeNextAsker NoticeAskerPacket((*Asker)->GetPlayerName(), 1);
						InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);

						ProviderTimer = 16;
						AskerTurn = true;
					}
					--ProviderTimer;
				}
			}

			//타이머 오류 제어 부분

			if (QuestionCount == 0)
			{
				TwentyRestTime restTimePacket;
				InGamePlayerManager.SendToAllPlayers(restTimePacket);
				Sleep(3000);
				//나가기 예약에 대한 처리를 해야함 -3초
				break;
			}

			//라운드 동작 while문을 탈출하는 경우는 이부분에 서술
		}
		++GameRound;
		Game_Setting_On();
		TwentyRoundEnd roundendPacket((Quiz_Provide_Player)->GetPlayerName(),(*Asker)->GetPlayerName(),GameRound);
		InGamePlayerManager.SendToAllPlayers(roundendPacket);
		//다음 질문자에 대한 정보를 보냄.
	}

	//점수 계산 및 게임 종료 사인
	std::sort(InGamePlayer.begin(), InGamePlayer.end(), &CompareScore);

	TwentyGameEnd gameEndPacket;
	for (int i = 0; i < InGamePlayer.size(); ++i) 
	{
		gameEndPacket.RankSortPlayer[i] = InGamePlayer[i]->GetPlayerName();
		gameEndPacket.RankScore[i] = InGamePlayer[i]->GetGameScore();
	}

	InGamePlayerManager.SendToAllPlayers(gameEndPacket);

}

//다음 질문자를 가리킴
void InGame::Next_Asker_Point()
{
	++Asker;
	if (Asker == AskerGroup.end())
	{
		Asker = AskerGroup.begin();
	}
}

void InGame::Connect_Check_In_Wait_time(int setTime)
{
	if (Quiz_Provide_Player == nullptr)
	{

	}
}

void InGame::Game_Setting_On()
{
	Quiz_Provide_Player = *(InGamePlayer.begin() + GameRound);

	for (int i = 0; i < InGamePlayer.size(); ++i)
	{
		if (InGamePlayer[i]->GetGPID() == Quiz_Provide_Player->GetGPID())
		{
			if (InGamePlayer[i]->GetGPID() == InGamePlayer.back()->GetGPID())
			{
				Asker = AskerGroup.begin();
			}
			else {
				AskerGroup.push_back(InGamePlayer[++i]);
				Asker = (AskerGroup.end() - 1);
			}
			continue;
		}
		AskerGroup.push_back(InGamePlayer[i]);
	}
}

TwentyProviderSelectAnswer InGame::SelectFiveAnswer(TwentyProviderSelectAnswer packet)
{
	std::random_device rn;
	std::mt19937_64 rnd(rn());
	std::shuffle(Words.begin(), Words.end(), rnd);

	for (int i = 0; i < 5; ++i)
	{
		packet.AnswerChoice[i] = *(Words.begin());
		Words.erase(Words.begin());
		Words.push_back(packet.AnswerChoice[i]);
	}

	return packet;
}

void InGame::SetPlayerPosition()
{
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(InGamePlayer.begin(), InGamePlayer.end(), g);
}

void InGame::AllPlayerReadyCheck()
{
	TwentyPlayerReadyCompletePacket packet;

	for (auto check : InGamePlayer)
	{
		do
		{
			RecvBuf = (*check).GetGamePacket();
			if(RecvBuf == nullptr) continue;
			packet.Deserialize(RecvBuf->buffer);
		} while (packet.getTwentyPacket() == Twenty_Packet_Type::Twenty_Player_Ready_Complete);
	}
}

void InGame::LoadingTime()
{
	time_t loading = time(NULL) + 1;
	while (loading != time(NULL))
	{
		continue;
	}
}

void InGame::TimerErrorProcess() {
	if (ActionTime != 0 && ActionTime + 2 <= time(NULL))
	{
		time_t range = time(NULL) - ActionTime;
		if (AskerTurn) {
			AskerTimer -= (int)range;
		}
		else {
			ProviderTimer -= (int)range;
		}
		ActionTime = time(NULL);
	}
}
