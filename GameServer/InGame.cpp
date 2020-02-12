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
	for (GameRound = 1; GameRound < InGamePlayer.size() + 1; ++GameRound) {

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

					TwentyAskerQuestion QuestionPacket;
					QuestionPacket.Deserialize(RecvBuf->buffer);
					TwentyAskerQuestionBroadCast QuestionBroadCastPacket(InGamePlayer[i]->GetPlayerName(), QuestionPacket.Question);
					InGamePlayerManager.SendToAllPlayers(QuestionBroadCastPacket);
					break;
				}
				case Twenty_Provider_Reply:
				{
					if ((*Quiz_Provide_Player)->GetGPID() != InGamePlayer[i]->GetGPID()) break;

					TwentyProviderReply ReplyPacket;
					ReplyPacket.Deserialize(RecvBuf->buffer);
					TwentyProviderReplyBroadCast ReplyBroadcastPacket(InGamePlayer[i]->GetPlayerName(), ReplyPacket.ReplyOX);
					InGamePlayerManager.SendToAllPlayers(ReplyBroadcastPacket);
					break;
				}
				case Twenty_Asker_Answer:
				{
					if ((*Asker)->GetGPID() != InGamePlayer[i]->GetGPID()) break;


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
	Quiz_Provide_Player = InGamePlayer.begin() + GameRound - 1;

	for (auto p : InGamePlayer)
	{
		if (p->GetGPID() == (*Quiz_Provide_Player)->GetGPID())
		{
			continue;
		}
		AskerGroup.push_back(p);
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

