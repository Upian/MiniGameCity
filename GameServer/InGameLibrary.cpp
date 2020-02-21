#include <WinSock2.h>
#include <time.h>
#include <random>
#include <algorithm>

#include "InGameLibrary.h"

/*
1. ������
2. ������ �Ҽ�
3. ��Ģ�� ����
4. ĳġ ���ε�
*/

bool CompareScore(const std::shared_ptr<Player> x, const std::shared_ptr<Player> y)
{
	return x->GetGameScore() > y->GetGameScore();
}

void InGameLibrary::TwentyQuestionGame(PlayerManager& _InGamePlayerManager) {

	if (_InGamePlayerManager.GetPlayerCount() <= 1)
	{
		return;
	}
	//���� �غ� ��Ʈ ���� ����
	BaseGameTypeSet = InGamePacketType::Twenty_Question_Game;
	InGamePlayerManager = _InGamePlayerManager;
	InGamePlayerList = InGamePlayerManager.GetPlayerList();
	InGamePlayer.reserve(5);
	AskerGroup.reserve(4);
	for (auto player : InGamePlayerList)
	{
		InGamePlayer.push_back(player);
	}

	//�÷��̾���� ������ �̿��Ͽ� ������ �ڸ���ġ�� ��
	//SetPlayerPosition();	//�׽�Ʈ�� ���� ��� �ּ� ó��

	TwentyPlayerLocationSettingPacket packet;
	for (int i = 0; i < InGamePlayer.size();++i)
	{
		char Utf8Player[31] = { NULL, };
		strcpy_s(Utf8Player, 31, conversion.ToUTF8((InGamePlayer[i]->GetPlayerName()).c_str()));
		strcpy_s(packet.PlayerName[i], 31, Utf8Player);
	}
	packet.PlayerCount = InGamePlayer.size();

	InGamePlayerManager.SendToAllPlayers(packet);

	//�÷��̾� ����ŭ ���� ������ ���� for��
	for (GameRound = 0; GameRound < InGamePlayer.size();) {

		//�÷��̾� ���� �غ� ����� ���� while��
		while (1)
		{
			AllPlayerReadyCheck();
			LoadingTime();

			//3�� ������ �˸��� ��Ŷ
			TwentyCountDownStart startpacket;
			InGamePlayerManager.SendToAllPlayers(startpacket);

			//������ ������ ����� �����ϴ°� �Ǵ��ؾ� �ϴ� �κ�(���� �̱���) - 3��
			Sleep(3000);
			//
			Game_Setting_On();

			//5���� ������ ������ ��Ŷ
			TwentyProviderSelectAnswer selectpacket;
			selectpacket = SelectFiveAnswer(selectpacket);
			(Quiz_Provide_Player)->SendPacket(selectpacket);

			//3�� ���Ḧ �˸��� ��Ŷ
			TwentyCountDownEnd endpacket;
			InGamePlayerManager.SendToAllPlayers(endpacket);

			//������ ������ ����� �����ϴ°� �Ǵ��ؾ� �ϴ� �κ�(���� �̱���) - 10��
			Sleep(5000);
			TwentyGameStart GameStartPacket;
			InGamePlayerManager.SendToAllPlayers(GameStartPacket);

			TwentySelectAnswer SelectedAnswer;
			do 
			{
				if(!PopGamePacketToQueue()) continue;
				if(RecvPacket.first->GetGPID() != (Quiz_Provide_Player)->GetGPID()) continue;
				SelectedAnswer.Deserialize(RecvPacket.second);
			} while ((int)SelectedAnswer.getTwentyPacket() != 6);

			strcpy_s(TwentyAnswer, 31, conversion.ToAnsi(SelectedAnswer.Answer));
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
			if (PopGamePacketToQueue())
			{
				Twenty_Packet_Type PacketType;
				PacketType = (Twenty_Packet_Type)PacketTypeDeserial(RecvPacket.second);

				switch (PacketType)
				{
				case Twenty_Packet_Type::Twenty_Asker_Question:
				{
					if ((*Asker)->GetGPID() != RecvPacket.first->GetGPID()) continue;

					//���� ������ �ٽ� �ѷ���
					TwentyAskerQuestion QuestionPacket;
					QuestionPacket.Deserialize(RecvPacket.second);
					char temp[31] = { NULL, };
					strcpy_s(temp, 31, conversion.ToAnsi(QuestionPacket.Question));
					TwentyAskerQuestionBroadCast QuestionBroadCastPacket(conversion.ToUTF8(RecvPacket.first->GetPlayerName().c_str()), conversion.ToUTF8(temp));
					InGamePlayerManager.SendToAllPlayers(QuestionBroadCastPacket);

					//���� ����Ƚ�� �ѷ���
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
					if (Quiz_Provide_Player->GetGPID() != RecvPacket.first->GetGPID()) continue;

					//���� �亯�� �ѷ���
					TwentyProviderReply ReplyPacket;
					ReplyPacket.Deserialize(RecvPacket.second);
					TwentyProviderReplyBroadCast ReplyBroadcastPacket(conversion.ToUTF8(RecvPacket.first->GetPlayerName().c_str()), ReplyPacket.ReplyOX);
					InGamePlayerManager.SendToAllPlayers(ReplyBroadcastPacket);

					if (QuestionCount != 0)
					{
						Next_Asker_Point();

						//���� �����ڸ� �˷���
						TwentyNoticeNextAsker NoticeAskerPacket(conversion.ToUTF8((*Asker)->GetPlayerName().c_str()), 0);
						InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);

						ProviderTimer = 15;
						AskerTurn = true;
						ActionTime = time(NULL);
					}
					else {
						TwentyAnswerOpen AnswerPacket;
						strcpy_s(AnswerPacket.Answer, 31, conversion.ToUTF8(TwentyAnswer));
						InGamePlayerManager.SendToAllPlayers(AnswerPacket);
					}

					break;
				}
				case Twenty_Packet_Type::Twenty_Asker_Answer:
				{
					if ((*Asker)->GetGPID() != RecvPacket.first->GetGPID()) continue;

					TwentyAskerAnswer AnswerPacket;
					AnswerPacket.Deserialize(RecvPacket.second);

					TwentyAskerAnswerBroadCast AnswerBroadCastPacket(AnswerPacket.AskerAnswer, conversion.ToUTF8(RecvPacket.first->GetPlayerName().c_str()), 0);

					char tempAnswer[31];
					strcpy_s(tempAnswer, 31, conversion.ToAnsi(AnswerPacket.AskerAnswer));
					if (strcmp(tempAnswer,TwentyAnswer) == 0)
					{
						//������ ������ ��� ����
						AnswerBroadCastPacket.AnswerResult = 1;
						InGamePlayerManager.SendToAllPlayers(AnswerBroadCastPacket);

						(Quiz_Provide_Player)->UpdateGameScore(QuestionCount * 2);
						(*Asker)->UpdateGameScore(QuestionCount * 5);

						TwentyUpdateScore providerScorePacket(conversion.ToUTF8((Quiz_Provide_Player)->GetPlayerName().c_str()), (Quiz_Provide_Player)->GetGameScore());
						TwentyUpdateScore askerScorerPacker(conversion.ToUTF8((*Asker)->GetPlayerName().c_str()), (*Asker)->GetGameScore());

						InGamePlayerManager.SendToAllPlayers(providerScorePacket);
						InGamePlayerManager.SendToAllPlayers(askerScorerPacker);

						ActionTime = 0;
						QuestionCount = 0;
					}
					else
					{
						//������ Ʋ���� ��� ����
						AnswerBroadCastPacket.AnswerResult = 0;
						InGamePlayerManager.SendToAllPlayers(AnswerBroadCastPacket);

						QuestionCount -= 1;

						TwentyRemainQuestion RemainCountPacket(QuestionCount);
						InGamePlayerManager.SendToAllPlayers(RemainCountPacket);

						if (QuestionCount != 0) {
							Next_Asker_Point();
							TwentyNoticeNextAsker NoticeAskerPacket(conversion.ToUTF8(RecvPacket.first->GetPlayerName().c_str()), 0);
							InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);

							AskerTimer = 30;
							ActionTime = time(NULL);
						}
						else
						{
							TwentyAnswerOpen AnswerPacket;
							strcpy_s(AnswerPacket.Answer, 31, conversion.ToUTF8(TwentyAnswer));
							InGamePlayerManager.SendToAllPlayers(AnswerPacket);
						}
					}

					break;
				}
				case Twenty_Packet_Type::Twenty_Exit_Reservation:
				{
					TwentyExitReservation exitReservationPacket;
					exitReservationPacket.Deserialize(RecvPacket.second);
					RecvPacket.first->SetExitReservation(exitReservationPacket.ReservationType);
					TwentyExitNotification exitNotificationPacket(conversion.ToUTF8(RecvPacket.first->GetPlayerName().c_str()), exitReservationPacket.ReservationType);

					break;
				}
				default:
				{
					break;
				}
				}
			}

			//timer ����
			TimerErrorProcess();

			if (ActionTime == time(NULL))
			{
				++ActionTime;
				TwentyInGameTimer InGameTimerPacket;
				if (AskerTurn)
				{
					InGameTimerPacket.Remaintime = AskerTimer;
					//InGamePlayerManager.SendToAllPlayers(InGameTimerPacket);
					if (AskerTimer <= 0)
					{
						AskerTimer = 31;
						QuestionCount -= 1;
						TwentyRemainQuestion RemainCountPacket(QuestionCount);
						InGamePlayerManager.SendToAllPlayers(RemainCountPacket);

						(*Asker)->UpdateGameScore(-5);
						TwentyUpdateScore AskerScorePacketr(conversion.ToUTF8((*Asker)->GetPlayerName().c_str()),(*Asker)->GetGameScore());
						InGamePlayerManager.SendToAllPlayers(AskerScorePacketr);

						if (QuestionCount != 0)
						{
							Next_Asker_Point();
							TwentyNoticeNextAsker NoticeAskerPacket(conversion.ToUTF8((*Asker)->GetPlayerName().c_str()), 1);
							InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);

							ActionTime = time(NULL);
						}
						else {
							TwentyAnswerOpen AnswerPacket;
							strcpy_s(AnswerPacket.Answer, 31, conversion.ToUTF8(TwentyAnswer));
							InGamePlayerManager.SendToAllPlayers(AnswerPacket);
						}
					}
					--AskerTimer;
				}
				else 
				{
					InGameTimerPacket.Remaintime = ProviderTimer;
					//InGamePlayerManager.SendToAllPlayers(InGameTimerPacket);

					if (ProviderTimer <= 0)
					{
						(Quiz_Provide_Player)->UpdateGameScore(-15);
						TwentyUpdateScore ProviderScorePacket(conversion.ToUTF8((Quiz_Provide_Player)->GetPlayerName().c_str()),(Quiz_Provide_Player)->GetGameScore());
						InGamePlayerManager.SendToAllPlayers(ProviderScorePacket);

						TwentyRemainQuestion RemainCountPacket(20);
						InGamePlayerManager.SendToAllPlayers(RemainCountPacket);

						Next_Asker_Point();
						TwentyNoticeNextAsker NoticeAskerPacket(conversion.ToUTF8((*Asker)->GetPlayerName().c_str()), 1);
						InGamePlayerManager.SendToAllPlayers(NoticeAskerPacket);
						ProviderTimer = 16;
						AskerTurn = true;
					}
					--ProviderTimer;
				}
			}


			if (QuestionCount == 0)
			{
				TwentyRestTime restTimePacket;
				InGamePlayerManager.SendToAllPlayers(restTimePacket);
				Sleep(3000);
				//������ ���࿡ ���� ó���� �ؾ��� -3��
				break;
			}

			//���� ���� while���� Ż���ϴ� ���� �̺κп� ����
		}
		++GameRound;
		if (GameRound >= InGamePlayerManager.GetPlayerCount()) break;
		Game_Setting_On();
		TwentyRoundEnd roundendPacket(conversion.ToUTF8((Quiz_Provide_Player)->GetPlayerName().c_str()),conversion.ToUTF8((*Asker)->GetPlayerName().c_str()),GameRound+1);
		InGamePlayerManager.SendToAllPlayers(roundendPacket);
		
		//���� �����ڿ� ���� ������ ����.
	}

	//���� ��� �� ���� ���� ����
	std::sort(InGamePlayer.begin(), InGamePlayer.end(), &CompareScore);

	TwentyGameEnd gameEndPacket;
	for (int i = 0; i < InGamePlayer.size(); ++i) 
	{
		strcpy_s(gameEndPacket.RankSortPlayer[i], 31, conversion.ToUTF8(InGamePlayer[i]->GetPlayerName().c_str()));
		gameEndPacket.RankScore[i] = InGamePlayer[i]->GetGameScore();
	}

	InGamePlayerManager.SendToAllPlayers(gameEndPacket);

	return;
}

//���� �����ڸ� ����Ŵ
void InGameLibrary::Next_Asker_Point()
{
	++Asker;
	if (Asker == AskerGroup.end())
	{
		Asker = AskerGroup.begin();
	}
}

void InGameLibrary::Connect_Check_In_Wait_time(int setTime)
{
	time_t ChechTime = time(NULL) + setTime;

	while (ChechTime != time(NULL))
	{
		for (auto i = InGamePlayer.begin(); i < InGamePlayer.end(); ++i)
		{

		}
	}

	if (Quiz_Provide_Player == nullptr)
	{

	}
}

//������ �����ϱ� ���� ���� ����
void InGameLibrary::Game_Setting_On()
{
	AskerGroup.clear();
	Quiz_Provide_Player = *(InGamePlayer.begin() + GameRound);

	for (auto i = InGamePlayer.begin(); i < InGamePlayer.end(); ++i)
	{
		if ((*i)->GetGPID() == Quiz_Provide_Player->GetGPID())
		{
			if ((*i)->GetGPID() == InGamePlayer.back()->GetGPID())
			{
				Asker = AskerGroup.begin();
			}
			else {
				AskerGroup.push_back(*(++i));
				Asker = (AskerGroup.end() - 1);
			}
			continue;
		}
		AskerGroup.push_back((*i));
	}
}

//5���� ������ �ܾ� ����
TwentyProviderSelectAnswer InGameLibrary::SelectFiveAnswer(TwentyProviderSelectAnswer packet)
{
	std::random_device rn;
	std::mt19937_64 rnd(rn());
	/*std::shuffle(Words.begin(), Words.end(), rnd);*/

	for (int i = 0; i < 5; ++i)
	{
// 		packet.AnswerChoice[i] = *(Words.begin());
// 		Words.erase(Words.begin());
// 		Words.push_back(packet.AnswerChoice[i]);
		strcpy_s(packet.AnswerChoice[i], 31, conversion.ToUTF8("����"));
	}

	return packet;
}

//�������� �÷��̾��� ��ġ ����
void InGameLibrary::SetPlayerPosition()
{
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(InGamePlayer.begin(), InGamePlayer.end(), g);
}

//��� �÷��̾ ������ ������ �غ� �Ǿ����� ��Ŷ�� �޾Ƽ� üũ
void InGameLibrary::AllPlayerReadyCheck()
{
	TwentyPlayerReadyCompletePacket packet;

	for (auto i = InGamePlayer.begin(); i < InGamePlayer.end();)
	{
		time_t CheckTime = time(NULL) + 5;
		do
		{
// 			if (CheckTime == time(NULL))
// 			{
// 				//������ ������ ���� ����� �������� �˷���(������)
// 				TwentyEscapeAnotherPlayer EscapePacket(conversion.ToUTF8((*i)->GetPlayerName().c_str()));
// 				InGamePlayerManager.PlayerDisconnect(*i);
// 				InGamePlayerManager.SendToAllPlayers(EscapePacket);
// 				i = InGamePlayer.erase(i);
// 				break;
// 			}

			if(!PopGamePacketToQueue()) continue;
			if (RecvPacket.first->GetGPID() != (*i)->GetGPID())
			{
				std::pair<std::shared_ptr<Player>, Buffer>* pPacket = new std::pair<std::shared_ptr<Player>, Buffer>(RecvPacket);
				PacketQueue.push(pPacket);
				continue;
			}
			packet.Deserialize(RecvPacket.second);
		} while (packet.getTwentyPacket() != Twenty_Packet_Type::Twenty_Player_Ready_Complete);
		++i;
	}
}

//ī��Ʈ�� ��Ȯ�� �����ϵ��� ��� ����ϵ��� ��.
void InGameLibrary::LoadingTime()
{
	time_t loading = time(NULL) + 1;
	while (loading != time(NULL))
	{
		continue;
	}
}

void InGameLibrary::TimerErrorProcess() {
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

void InGameLibrary::SaveGamePacketToQueue(Buffer& buffer, std::shared_ptr<Player> player)
{
	if (nullptr == player) return;

	InGamePacketType type = (InGamePacketType)PacketTypeDeserial(buffer);

	/*if (type != BaseGameTypeSet) return;*/

	std::pair<std::shared_ptr<Player>, Buffer> * temp = new std::pair<std::shared_ptr<Player>, Buffer>();
	temp->first = player;
	temp->second = buffer;

	PacketQueue.push(temp);
}

bool InGameLibrary::PopGamePacketToQueue()
{
	if (PacketQueue.empty()) return false;

	RecvPacket = make_pair(PacketQueue.front()->first, PacketQueue.front()->second);
	delete PacketQueue.front();
	PacketQueue.pop();

	return true;
}