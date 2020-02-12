#include <WinSock2.h>
#include <time.h>
#include <random>

#include "InGame.h"

/*
1. ������
2. ������ �Ҽ�
3. ��Ģ�� ����
4. ĳġ ���ε�
*/

void InGame::TwentyQuestionGame(PlayerManager& InGamePlayerManager) {

	//���� �غ� ��Ʈ ���� ����
	InGamePlayerList = InGamePlayerManager.GetPlayerList();
	for (auto player : InGamePlayerList)
	{
		InGamePlayer.push_back(player);
	}
	//�÷��̾���� ������ �̿��Ͽ� ������ �ڸ���ġ�� ��
	SetPlayerPosition();

	TwentyPlayerLocationSettingPacket packet;
	for (int i = 0; i < InGamePlayer.size();++i)
	{
		packet.PlayerName[i] = InGamePlayer[i]->GetPlayerName();
	}
	packet.PlayerCount = InGamePlayer.size();

	InGamePlayerManager.SendToAllPlayers(packet);

	//�÷��̾� ����ŭ ���� ������ ���� for��
	for (GameRound = 0; GameRound < InGamePlayer.size(); ++GameRound) {

		//�÷��̾� ���� �غ� ����� ���� while��
		while (1)
		{
			AllPlayerReadyCheck();
			LoadingTime();
			TwentyCountDownStart startpacket;
			InGamePlayerManager.SendToAllPlayers(startpacket);

			//������ ������ ����� �����ϴ°� �Ǵ��ؾ� �ϴ� �κ�(���� �̱���) - 3��
			Sleep(3000);
			Game_Setting_On();

			TwentyProviderSelectAnswer selectpacket;
			selectpacket = SelectFiveAnswer(selectpacket);
			(*Quiz_Provide_Player)->SendPacket(selectpacket);

			TwentyCountDownEnd endpacket;
			InGamePlayerManager.SendToAllPlayers(endpacket);

			//������ ������ ����� �����ϴ°� �Ǵ��ؾ� �ϴ� �κ�(���� �̱���) - 10��
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

		//�ΰ��� ������ ����� ���� ����
		QuestionCount = 20;
		AskerTimer = 30;
		ProviderTimer = 15;
		AskerTurn = true;
		ActionTime = time(NULL);

		//���� �� Ÿ�̸Ӹ� �ݺ��ϱ� ���� while��
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

					//���� ������ �ٽ� �ѷ���
					TwentyAskerQuestion QuestionPacket;
					QuestionPacket.Deserialize(RecvBuf->buffer);
					TwentyAskerQuestionBroadCast QuestionBroadCastPacket(InGamePlayer[i]->GetPlayerName(), QuestionPacket.Question);
					InGamePlayerManager.SendToAllPlayers(QuestionBroadCastPacket);

					//���� ����Ƚ�� �ѷ���
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

					//���� �亯�� �ѷ���
					TwentyProviderReply ReplyPacket;
					ReplyPacket.Deserialize(RecvBuf->buffer);
					TwentyProviderReplyBroadCast ReplyBroadcastPacket(InGamePlayer[i]->GetPlayerName(), ReplyPacket.ReplyOX);
					InGamePlayerManager.SendToAllPlayers(ReplyBroadcastPacket);

					Next_Asker_Point();

					//���� �����ڸ� �˷���
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
						//������ ������ ��� ����

					}
					else
					{
						 //������ Ʋ���� ��� ����
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
			//recv ���� �� switch��

			//timer ����
		}

		//���� ������ ����
		++Quiz_Provide_Player;
	}
}

//���� �����ڸ� ����Ŵ
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
	//��Ŷ �ۼ� �� ������ ����
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

