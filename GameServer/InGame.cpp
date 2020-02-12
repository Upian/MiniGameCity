#include <WinSock2.h>
#include <time.h>
#include <random>

#include "InGame.h"

/*
1. 스무고개
2. 릴레이 소설
3. 금칙어 게임
4. 캐치 마인드
*/

void InGame::TwentyQuestionGame(PlayerManager& InGamePlayerManager) {

	//게임 준비 파트 변수 생성
	InGamePlayerList = InGamePlayerManager.GetPlayerList();
	for (auto player : InGamePlayerList)
	{
		InGamePlayer.push_back(player);
	}
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
	for (GameRound = 0; GameRound < InGamePlayer.size(); ++GameRound) {

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
			(*Quiz_Provide_Player)->SendPacket(selectpacket);

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
				RecvBuf = (*Quiz_Provide_Player)->GetGamePacket();
				if (SelectedAnswer.AnswerCheck(RecvBuf->buffer))
				{
					TwentyAnswer = SelectedAnswer.Answer;
					break;
				}
			}
			break;
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

				InGamePacketType GameType;
				GameType = (InGamePacketType)PacketTypeDeserial(RecvBuf->buffer);

				if (GameType != Twenty_Question_Game) break;

				Twenty_Packet_Type PacketType;
				PacketType = (Twenty_Packet_Type)PacketTypeDeserial(RecvBuf->buffer);

				switch (PacketType)
				{
				case Twenty_Asker_Question:
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

					break;
				}
				case Twenty_Provider_Reply:
				{
					if ((*Quiz_Provide_Player)->GetGPID() != InGamePlayer[i]->GetGPID()) break;

					//받은 답변을 뿌려줌
					TwentyProviderReply ReplyPacket;
					ReplyPacket.Deserialize(RecvBuf->buffer);
					TwentyProviderReplyBroadCast ReplyBroadcastPacket(InGamePlayer[i]->GetPlayerName(), ReplyPacket.ReplyOX);
					InGamePlayerManager.SendToAllPlayers(ReplyBroadcastPacket);

					Next_Asker_Point();

					//다음 질문자를 알려줌
					TwentyNoticeNextAsker NoticeAskerPacket(InGamePlayer[i]->GetPlayerName(),0);
					InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);
					

					break;
				}
				case Twenty_Asker_Answer:
				{
					if ((*Asker)->GetGPID() != InGamePlayer[i]->GetGPID()) break;

					TwentyAskerAnswer AnswerPacket;
					AnswerPacket.Deserialize(RecvBuf->buffer);

					if (AnswerPacket.AskerAnswer == TwentyAnswer)
					{
						//정답을 맞췄을 경우 동작

					}
					else
					{
						 //정답을 틀렸을 경우 동작
					}

					break;
				}
				case Twenty_Exit_Reservation:
				{
					break;
				}
				default:
				{
					break;
				}
					
				}
			}
			//recv 받을 시 switch문

			//timer 셋팅
		}

		//다음 출제자 지정
		++Quiz_Provide_Player;
	}
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

void InGame::Connect_Check_In_Wait_time()
{
	//패킷 작성 후 데이터 대입
}

void InGame::Game_Setting_On()
{
	Quiz_Provide_Player = InGamePlayer.begin() + GameRound;

	for (int i = 0; i < InGamePlayer.size(); ++i)
	{
		if (InGamePlayer[i]->GetGPID() == (*Quiz_Provide_Player)->GetGPID())
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
			RecvBuf = nullptr;
			RecvBuf = (*check).GetGamePacket();
		} while (packet.PlayerReadyCheck(RecvBuf->buffer));
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

void InGame::ScoreUpdate(int update, std::shared_ptr<Player> player)
{
	TwentyUpdateScore updatePacket(player->GetPlayerName(), update);
}

