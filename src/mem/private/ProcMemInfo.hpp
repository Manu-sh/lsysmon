#pragma once

#include "../../utils/utils.hpp"

#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <regex>

// TODO
namespace Proc {

	struct MemInfo {

		MemInfo();
	};
	
}

Proc::MemInfo::MemInfo() {

	utils::Line::ifstream_l meminfo{"/proc/meminfo"};
	meminfo.default_exceptions();
	meminfo.each_line([](const auto &l){ cout << l << endl; });

}