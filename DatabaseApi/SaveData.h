#ifndef __DB_SAVE_H__
#define __DB_SAVE_H__

#include "BasePacket.h"

namespace db {
	enum Table {
		tableNone = 0,

		tableLogin,
		tableUser,
		tableInventory,
		tableGame,
		tableFriend,
		tableGuild,
		tableRanking,

		tableSize,
	};

	struct Login {
		uint32 gpid = 0;
		std::string userId;
		std::string userPw;
		std::string userStatus;
		std::string dateCreated;
		std::string dateLogin;
		std::string dateLogout;
	};

	struct User {
		uint32 gpid = 0;
		int32 gold = 0;
		int32 ranking = 0;
	};

	struct Friend {
		uint32 friendGpid = 0;
		std::string friendName;
		std::string modifyDate;
	};

	struct Friends {
		uint32 gpid = 0;
		std::list<Friend> friends;
	};

	struct Guild {
		uint32 guildId = 0;
		std::string guildName;
		std::string guildMaster;
		std::string guildMember;
		std::string guildLimit;
	};

	struct Game {
		uint32 gpid;
		int32 point;
	};

	/*template<typename... Args>
	void SaveData(const Command command, const Table table, Args... args) {
		DBCacheSave packet;
		packet.command = command;
		packet.table = table;

	}*/

	typedef int TICKET;

	//// update, delete, insert,
	//template<typename... Args>
	//TICKET SaveData(const Table table, Args... args) {
	//	// serialize해서 보내기
	//	DBCacheLoad packet;
	//	
	//	for (int i = 0; i < )
	//	switch (command) {
	//	/*case commandInsert: {
	//		query = "INSERT INTO ";
	//		query += table;
	//		query += " (";
	//		switch (table) {
	//		case tableLogin: {
	//			const int size = sizeof...(args);
	//			for (int i = 0; i < size; ++i) {

	//			}
	//			break;
	//		}
	//		case tableUser: {

	//			break;
	//		}
	//		case tableInventory: {

	//			break;
	//		}
	//		case tableGame: {

	//			break;
	//		}
	//		case tableFriend: {

	//			break;
	//		}
	//		case tableGuild: {

	//			break;
	//		}
	//		case tableGuild: {

	//			break;
	//		}
	//		case tableRanking: {

	//			break;
	//		}
	//		}

	//		query += ")";
	//		break;
	//	}
	//	case commandUpdate: {
	//		query = "UPDATE ";
	//		query += table;
	//		query += " SET ";
	//		const int size = sizeof...(args);
	//		for (int i = 0; i < size; ++i) {

	//		}

	//		break;
	//	}
	//	case commandDelete: {
	//		query = "DELETE ";
	//		query += "FROM ";
	//		query += table;
	//		const int size = sizeof...(args);
	//		for (int i = 0; i < size; ++i) {

	//		}
	//		query += "GPID=";

	//		break;
	//	}*/
	//	default:
	//		break;
	//	}
	//	return 0;
	//}
}

#endif // __DB_SAVE_H__