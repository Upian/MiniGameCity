#pragma once
#ifndef __BASELIB_LOG_H__
#define __BASELIB_LOG_H__
#include "Singleton.h"
#include <fstream>
#include <string>

/*
*	First create singleton class
*	use Logging function
*/
namespace Util {
	enum LogLevel {
		logLevelNone = 0,

		logLevelFatal,
		logLevelError,
		logLevelInfo,
		logLevelDebug,

		logLevelCount,

	};
	class LogManager :public Singleton<LogManager> {
	public:
		void SetDirectory(const char* directory) { m_directory = directory; }
		std::string GetDirectory() const { return m_directory; }
		std::ofstream* GetFileStream() const { return m_writeFile; }

		void SetTrueIsFolderExist() { m_isFolderExist = true; }
		bool GetIsFolderExist() const { return m_isFolderExist; }

		void SetLogLevel(LogLevel logLevel) { m_logLevel = logLevel; }
		LogLevel GetLogLevel() const { return m_logLevel; }

		void SetActiveConsoleLog(bool value) { m_isActiveConsoleLog = value; }
		bool GetActiveConsoleLog() const { return m_isActiveConsoleLog; }
		void SetActiveLocalLog(bool value) { m_isActiveLocalLog; }
		bool GetActiveLocalLog() const { return m_isActiveLocalLog; }
	private:
		DECLARE_SINGLETON(LogManager)

//		LogManager(const char* directory, int logLevel = LogLevel::logLevelDebug, bool isActiveConsoleLog = true, bool isActiveLocalLog = true);
		std::string m_directory = "../../../Logs/";
		std::ofstream* m_writeFile;// = nullptr;
		bool m_isFolderExist = false;

		bool m_isActiveConsoleLog = true;
		bool m_isActiveLocalLog = true;

		LogLevel m_logLevel = LogLevel::logLevelNone;
	};

	/*
	Logging(file name, message, ...)
	If you do not write file name, it will be only displayed in console window.
	*/
	void Logging(LogLevel logLevel, std::string fileName, const char* msg, ...);
	void LoggingFatal(std::string fileName, const char* msg, ...);
	void LoggingError(std::string fileName, const char* msg, ...);
	void LoggingInfo(std::string fileName, const char* msg, ...);
	void LoggingDebug(std::string fileName, const char* msg, ...);
}



#endif //!__BASELIB_LOG_H__
