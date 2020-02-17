#pragma once
#ifndef __BASELIB_ERROR_TYPE_H__
#define __BASELIB_ERROR_TYPE_H__

enum class ErrorTypeAddFriend : char {
	none = 0,

	notExistPlayer,
	srcFriendListIsFull, 
	destFriendListIsFull,
	destFriendRequestListIsFull,
	alreadySendRequest,
	samePlayer,
	alreadyFriends,

	count,
};

enum class ErrorTypeAcceptFriend : char {
	none = 0,

	notHaveFriendRequestList,
	srcFriendListIsFull,
	destFriendListIsFull,

	count,
};

enum class ErrorTypeEnterRoom : char {
	errorTypeNone = 0,

	errorTypeNotExistRoom,
	errorTypeWrongPassword,
	errorTypeAlreadyIncluded,
	errorTypeGameStart,
	errorTypeMaxPlayer,
	errorTypeCanNotEnterRoom,
	errorTypePlayerLogout,

	errorTypeCount,
};

enum class ErrorTypeStartGame : char {
	errorTypeStartGameNone = 0,

	errorTypeStartGameReady, //Someone do not press ready buddon
	errorTypeStartGameNotHaveGame,
	errorTypeStartGameAlreadyStartGame,
	errorTypeStartGameRoomIsNotWaitingGame,

	errorTypeStartGameCount,
};
#endif // !__BASELIB_ERROR_TYPE_H__
