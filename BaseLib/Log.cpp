#include "Log.h"
#include <cstdarg>
namespace Util {
	LogManager::LogManager() {
		m_writeFile = new std::ofstream();
	}

	LogManager::LogManager(const char* directory) :m_directory(directory) {
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

		printf("Log message: %s", msgBuf);
		if (true == fileName.empty()) return;

		Util::LogManager* logger = Util::LogManager::GetSingleton();
		std::ofstream* fout = logger->GetFileStream();
		fout->open(logger->GetDirectory() + fileName, std::ios_base::out | std::ios_base::app);
		*fout << msgBuf << std::endl;
		fout->close();

		return;
	}
}
