#ifndef SOCIALPACKET_H
#define	SOCIALPACKET_H
#include "BasePacket.h"

enum SocialPacketType {
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
	SocialPacket() {}
	~SocialPacket() {}
	virtual void Serialize(SocialPacket* _packet, char* _buf);
	virtual void Deserialize(char* _buf, SocialPacket* _packet);
private:
	int chatType = 0;
	char data[BUFFER_SIZE] = "";
};

#endif