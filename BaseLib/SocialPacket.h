#ifndef SOCIALPACKET_H
#define	SOCIALPACKET_H
#include "BasePacket.h"

enum SocialPacketType {
	socialPacketTypeNone = 0,
	socialPacketTypeFriendAddResponse,  //(bool)yes or no
	socialPacketTypeFriendAddRequest, //(string)from, (string)to
	socialPacketTypeFriendDeleteResponse, //(bool)yes or no
	socialPacketTypeFriendDeleteRequest, //(string)id
	socialPacketTypeFriendFollowResponse, //(bool)yes or no, 그러면 id가 어딧는지 정보를 알고 있으니 가능한지 여부
	socialPacketTypeFriendFollowRequest, //(string)from, (string)to
	socialPacketTypeGuildCreateResponse, //(bool)yes or no
	socialPacketTypeGuildCreateRequest, //(string)id, (string)guildname, 다른 조건들(돈이나 등등..)
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
	SocialPacket() {}
	~SocialPacket() {}
	void SetSocialPacketType(SocialPacketType _type) {
		socialPacketType = _type;
		//basePacketType = BasePacketType::basePacketTypeSocial;
	}
	SocialPacketType GetSocialPacketType() {
		return socialPacketType;
	}
private:
	SocialPacketType socialPacketType = socialPacketTypeNone;
};

class ChatAllRequest : public SocialPacket {
public:
	ChatAllRequest() {}
	~ChatAllRequest() {}
	virtual void Serialize(char* _buf) override;
	virtual void Deserialize(char* _buf) override;
	char* GetChatAllRequest() {
		return chatAllRequest;
	}
	void SetChatAllRequest(char* _buf) {
		*chatAllRequest = *_buf;
	}
private:
	char chatAllRequest[BUFFER_SIZE] = "";
};

#endif