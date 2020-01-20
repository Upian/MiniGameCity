#pragma once
#ifndef __BASELIB_CONFIG_H__
#define __BASELIB_CONFIG_H__

#include "Singleton.h"
#include <string>

/*
*	First, create singleton class
*	f you create a config file, create it only in the Config directory. 
*	The path starts from the Config directory.
*
*	use GetConfig... function
*/
namespace Util {

	class ConfigManager :public Singleton<ConfigManager> {
	public:
		void SetDirectory(const char* directory) { m_directory.append(directory); }
		std::string GetDirectory() const { return m_directory; }

	private:
		DECLARE_SINGLETON(ConfigManager)
		ConfigManager(const char* directory);
		std::string m_directory = "../../../Config/";

	};

	/*
	*	ini file �̸�
	*	section �̸�
	*	key �̸�
	*	���� ��� �⺻��
	*/
	int GetConfigToInt(const char* fileName, const char* section, const char* key, int defaultValue = 0);
	std::string GetConfigToString(const char* fileName, const char* section, const char* key, const char* defaultValue = "");
	void GetConfigToString(const char* fileName, const char* section, const char* key, std::string& buffer, const char* defaultValue = "");
	bool GetConfigToBoolean(const char* fileName, const char* section, const char* key, bool defaultValue = false);

} //!Util


#endif //!__BASELIB_CONFIG_H__