#include "Log.h"
#include <cstdarg>
#include <ctime>
#include <string>
#include <cstdio>
#include <direct.h>
#include <cerrno>

namespace Util {
	LogManager::LogManager() {
		m_writeFile = new std::ofstream();
	}

	LogManager::LogManager(const char* directory, bool isActiveConsoleLog /* = true */, bool isActiveLocalLog /* = true*/) :
		m_directory(directory),
		m_isActiveConsoleLog(isActiveConsoleLog),
		m_isActiveLocalLog(isActiveLocalLog){
		m_writeFile = new std::ofstream();
	}

	LogManager::~LogManager() {
		if (nullptr != m_writeFile) {
			delete m_writeFile;
			m_writeFile = nullptr;
		}
	}


	void Util::Logging(std::string fileName, const char* msg, ...) {
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

		Util::LogManager* logger = Util::LogManager::GetSingleton();
		if (nullptr == logger)
			logger = Util::LogManager::CreateSingleton();

		//Console log
		if (true == logger->GetActiveConsoleLog()) 
			printf("%s\tLog message: %s\n", timeToString, msgBuf); //display console log
		
		
		//Local log
		if (true == fileName.empty()) return;
		if (false == logger->GetActiveLocalLog()) return;

		std::ofstream* fout = logger->GetFileStream();

		//Make directory
		char date[20];
		sprintf_s(date, sizeof(date), "%d.%d.%d", 
			currTimeFormat.tm_year + 1900,
			currTimeFormat.tm_mon + 1, currTimeFormat.tm_mday );
		int result = 0;
		if (false == logger->GetIsFolderExist()) {
			result = _mkdir(logger->GetDirectory().c_str());
			if(0 == result || EEXIST == errno)
				result = _mkdir((logger->GetDirectory() + date).c_str());
			if (0 == result || EEXIST == errno)
				logger->SetTrueIsFolderExist();
		}
	
		if (0 == result || EEXIST == errno) {
			fout->open(logger->GetDirectory() + date + "/" + fileName, std::ios_base::out | std::ios_base::app);
			*fout << timeToString << "\t" << msgBuf << std::endl;
			fout->close();
		}
	
		return;
	}
}
