#pragma once
#ifndef __BASELIB_DESIGN_DATA_H__
#define __BASELIB_DESIGN_DATA_H__

/*
*	정의할 각 Design data에 해당 클래스 상속
*/

#include <vector>
#include <string>

namespace Util {
	class DesignData {
	public:
		std::vector<std::string> LoadDesignData();
	private:

	};
}

#endif //!__UTIL_DESIGN_DATA_H__