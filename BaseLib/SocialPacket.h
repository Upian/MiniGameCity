#ifndef SOCIALPACKET_H
#define	SOCIALPACKET_H
#include "BasePacket.h"

enum SocialPacketType : char {
	socialPacketTypeNone = 0,
	socialPacketTypeFriendAddResponse,  //(bool)yes or no
	socialPacketTypeFriendAddRequest, //(string)from, (string)to
	socialPacketTypeFriendDeleteResponse, //(bool)yes or no
	socialPacketTypeFriendDeleteRequest, //(string)id
	socialPacketTypeFriendFollowResponse, //(bool)yes or no, �׷��� id�� ������� ������ �˰� ������ �������� ����
	socialPacketTypeFriendFollowRequest, //(string)from, (string)to
	socialPacketTypeGuildCreateResponse, //(bool)yes or no
	socialPacketTypeGuildCreateRequest, //(string)id, (string)guildname, �ٸ� ���ǵ�(���̳� ���..)
	socialPacketTypeGuildDeleteResponse, //(bool)yes or no
	socialPacketTypeGuildDeleteRequest, //(string)id, (string)guildname
	socialPacketTypeGuildInviteResponse, //(bool)yes or no
	socialPacketTypeGuildInviteRequest, //(string)from, (string)to, (string)guild
	socialPacketTypeChatResponse, //(string)from
	socialPacketTypeChatAllRequest, //(string)id
	socialPacketTypeChatWhisperRequest, //(string)from, (string)to, 
	socialPacketTypeChatFriendRequest, //(string)from, (string)to
	socialPacketTypeChatGuildRequeset, //(string)from, (string)to
};

class SocialPacket : public BasePacket {
public:
	SocialPacket() {
		SetBasePacketType(BasePacketType::basePacketTypeSocial);
	}
	~SocialPacket() {}
	void SetSocialPacketType(SocialPacketType _type) {
		socialPacketType = _type;
	}
	SocialPacketType GetSocialPacketType() {
		return socialPacketType;
	}
private:
	SocialPacketType socialPacketType = socialPacketTypeNone;
};

class ChatAllRequest : public SocialPacket {
public:
	ChatAllRequest() {
		SetSocialPacketType(SocialPacketType::socialPacketTypeChatAllRequest);
	}
	~ChatAllRequest() {}
	virtual Buffer& Serialize() override;
	virtual void Deserialize(char* _buf) override;
	char* GetChatAllRequest() {
		return chatAllRequest;
	}
	void SetChatAllRequest(char* _buf) {
		int len = strlen(_buf) + 1;
		memcpy(chatAllRequest, _buf, len);
	}
private:
	char chatAllRequest[BUFFER_SIZE - 2 * sizeof(char)]{};
};

#endif