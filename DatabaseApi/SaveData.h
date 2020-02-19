#ifndef __DB_SAVE_H__
#define __DB_SAVE_H__

namespace Database {

	enum Command {
		commandNone = 0,

		commandSelect,

		commandInsert,
		commandUpdate,
		commandDelete,

		commnadSize,
	};

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

	/*template<typename... Args>
	void SaveData(const Command command, const Table table, Args... args) {
		DBCacheSave packet;
		packet.command = command;
		packet.table = table;

	}*/


	//// update, delete, insert,
	template<typename... Args>
	void SaveData(const Command command, const Table table, Args... args) {
		switch (command) {
		/*case commandInsert: {
			query = "INSERT INTO ";
			query += table;
			query += " (";
			switch (table) {
			case tableLogin: {
				const int size = sizeof...(args);
				for (int i = 0; i < size; ++i) {

				}
				break;
			}
			case tableUser: {

				break;
			}
			case tableInventory: {

				break;
			}
			case tableGame: {

				break;
			}
			case tableFriend: {

				break;
			}
			case tableGuild: {

				break;
			}
			case tableGuild: {

				break;
			}
			case tableRanking: {

				break;
			}
			}

			query += ")";
			break;
		}
		case commandUpdate: {
			query = "UPDATE ";
			query += table;
			query += " SET ";
			const int size = sizeof...(args);
			for (int i = 0; i < size; ++i) {

			}

			break;
		}
		case commandDelete: {
			query = "DELETE ";
			query += "FROM ";
			query += table;
			const int size = sizeof...(args);
			for (int i = 0; i < size; ++i) {

			}
			query += "GPID=";

			break;
		}*/
		default:
			break;
		}

	}
}

#endif // __DB_SAVE_H__