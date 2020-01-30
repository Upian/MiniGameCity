#include <cstdarg>
#include <ctime>
#include <string>
#include <cstdio>
#include <direct.h>
#include <cerrno>
#include "Config.h"
#include "Log.h"

namespace {
	void Log(std::string fileName, const char* msg) {

	}
}
namespace Util {
	LogManager::LogManager() : 
		m_writeFile(new std::ofstream()),
		m_directory(Util::GetConfigToString("BaseServer.ini", "Definition", "LogDir", "../../../Logs/").c_str()),
		m_logLevel(static_cast<LogLevel>(Util::GetConfigToInt("BaseServer.ini", "Definition", "LogLevel", 4))),
		m_isActiveConsoleLog(Util::GetConfigToBoolean("BaseServer.ini", "Definition", "ActiveConsoleLog", true)),
		m_isActiveLocalLog(Util::GetConfigToBoolean("BaseServer.ini", "Definition", "ActiveLocalLog", true))
	{
	}

	LogManager::~LogManager() {
		if (nullptr != m_writeFile) {
			delete m_writeFile;
			m_writeFile = nullptr;
		}
	}

	void Util::Logging(LogLevel logLevel, std::string fileName, const char* msg, ...) {
		if (LogLevel::logLevelNone > logLevel &&
			LogLevel::logLevelCount < logLevel)
			return;

		Util::LogManager* logger = Util::LogManager::GetInstance();
		if (nullptr == logger)
			return;

		if (logger->GetLogLevel() < logLevel)
			return;

		int len = 0;
		char msgBuf[1024] = { 0, };
		va_list ap;
		va_start(ap, msg);
		len = vsprintf_s(msgBuf, 1024, msg, ap);
		msgBuf[len] = '\0';
		va_end(ap);

		time_t currTime = 0;
		struct tm currTimeFormat;
		currTime = time(NULL);
		localtime_s(&currTimeFormat, &currTime);

		char timeToString[50];
		sprintf_s(timeToString, sizeof(timeToString), "%4d/%2d/%2d %2d:%2d:%2d", 
			currTimeFormat.tm_year+1900,
			currTimeFormat.tm_mon + 1, currTimeFormat.tm_mday,
			currTimeFormat.tm_hour, currTimeFormat.tm_min, currTimeFormat.tm_sec);

		

		//Console log
		if (true == logger->GetActiveConsoleLog())
			printf("%s\tLog message: %s\n", timeToString, msgBuf); //display console log


		//Local log
		if (true == fileName.empty()) return;
		if (false == logger->GetActiveLocalLog()) return;

		//Make directory
		char date[20];
		sprintf_s(date, sizeof(date), "%d.%d.%d",
			currTimeFormat.tm_year + 1900,
			currTimeFormat.tm_mon + 1, currTimeFormat.tm_mday);
		int result = 0;

		//Folder duplicate check
		if (false == logger->GetIsFolderExist()) {
			result = _mkdir(logger->GetDirectory().c_str());
			if (0 == result || EEXIST == errno)
				result = _mkdir((logger->GetDirectory() + date).c_str());
			if (0 == result || EEXIST == errno)
				logger->SetTrueIsFolderExist();
		}

		//Write local txt file  #Thread_SAFE
		std::ofstream* fout = logger->GetFileStream();
		if (nullptr == fout) return;

		if (0 == result || EEXIST == errno) {
			fout->open(logger->GetDirectory() + date + "/" + fileName, std::ios::app/* std::ios_base::out | std::ios_base::app*/);
			*fout << timeToString << "\t" << msgBuf << std::endl;
			if (true == fout->is_open())
				fout->close();
		}


		return;
	}

	void LoggingFatal(std::string fileName, const char* msg, ...) {
		int len = 0;
		char msgBuf[1024] = { 0, };

		va_list ap;
		va_start(ap, msg);
		len = vsprintf_s(msgBuf, 1024, msg, ap);
		msgBuf[len] = '\0';
		va_end(ap);

		Logging(LogLevel::logLevelFatal, fileName, msgBuf);
	}
	void LoggingError(std::string fileName, const char* msg, ...) {
		int len = 0;
		char msgBuf[1024] = { 0, };

		va_list ap;
		va_start(ap, msg);
		len = vsprintf_s(msgBuf, 1024, msg, ap);
		msgBuf[len] = '\0';
		va_end(ap);

		Logging(LogLevel::logLevelError, fileName, msgBuf);
	}
	void LoggingInfo(std::string fileName, const char* msg, ...) {
		int len = 0;
		char msgBuf[1024] = { 0, };

		va_list ap;
		va_start(ap, msg);
		len = vsprintf_s(msgBuf, 1024, msg, ap);
		msgBuf[len] = '\0';
		va_end(ap);

		Logging(LogLevel::logLevelInfo, fileName, msgBuf);
	}
	void LoggingDebug(std::string fileName, const char* msg, ...) {
		int len = 0;
		char msgBuf[1024] = { 0, };

		va_list ap;
		va_start(ap, msg);
		len = vsprintf_s(msgBuf, 1024, msg, ap);
		msgBuf[len] = '\0';
		va_end(ap);

		Logging(LogLevel::logLevelDebug, fileName, msgBuf);
	}
}
