#include "Config.h"
#include <Windows.h>
#include "Log.h"

namespace Util{
	ConfigManager::ConfigManager() {}
	ConfigManager::ConfigManager(const char* directory) : m_directory(directory) {}
	ConfigManager::~ConfigManager() {}

	int GetConfigToInt(const char* fileName, const char* section, const char* key, int defaultValue /* = 0 */) {
		if ("" == fileName) {
			Logging("Config.log", "error - Config file not exist");
			return 0;
		}

		ConfigManager* config = ConfigManager::GetSingleton();
		if (nullptr == config) return 0;
		
		std::string directory = config->GetDirectory().append(fileName);

		return GetPrivateProfileInt(section, key, defaultValue, directory.c_str());
	}

	//값 반환
	std::string GetConfigToString(const char* fileName, const char* section, const char* key, const char* defaultValue /* = "" */) {
		if ("" == fileName) {
			Logging("Config.log", "error - Config file not exist");
			return "";
		}
		
		ConfigManager* config = ConfigManager::GetSingleton();
		if (nullptr == config) return nullptr;
		
		std::string directory = config->GetDirectory().append(fileName);

		char returnBuffer[256] = { 0, };
		GetPrivateProfileString(section, key, defaultValue, returnBuffer, sizeof(returnBuffer), directory.c_str());

		return returnBuffer;
	}

	//buffer를 통해 반환
	void GetConfigToString(const char* fileName, const char* section, const char* key, std::string& buffer, const char* defaultValue /* = "" */) {
		if ("" == fileName) {
			Logging("Config.log", "error - Config file not exist");
			buffer.clear();
			buffer = defaultValue;
			return;
		}
		
		ConfigManager* config = ConfigManager::GetSingleton();
		if (nullptr == config) return;
		
		std::string directory = config->GetDirectory().append(fileName);

		char returnBuffer[256] = { 0, };
		GetPrivateProfileString(section, key, defaultValue, returnBuffer, sizeof(returnBuffer), directory.c_str());
		
		buffer.clear();
		buffer = returnBuffer;

		return;
	}










}