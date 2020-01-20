#pragma once
#ifndef __BASELIB_DESIGN_DATA_MANAGER_H__
#define __BASELIB_DESIGN_DATA_MANAGER_H__

#include <map>

class DesignData;

namespace Util {
	class DesignDataManager {
	public:


	private:
		std::map<int, DesignData> m_dataList;
		std::string m_directory;

	};
}



#endif //!__BASELIB_DESIGN_DATA_MANAGER_H__