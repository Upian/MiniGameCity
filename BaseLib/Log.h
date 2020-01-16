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
	class LogManager :public Singleton<LogManager> {
	public:
		void SetDirectory(const char* directory) { m_directory = directory; }
		std::string GetDirectory() const { return m_directory; }
		std::ofstream* GetFileStream() const { return m_writeFile; }

	private:
		DECLARE_SINGLETON(LogManager)
		LogManager(const char* directory);
		std::string m_directory;
		std::ofstream* m_writeFile = nullptr;
	};

	/*
	Logging(file name, message, ...)
	If you do not write file name, it will be only displayed in console window.
	*/
	void Logging(std::string fileName, const char* msg, ...);
}



#endif //!__BASELIB_LOG_H__
