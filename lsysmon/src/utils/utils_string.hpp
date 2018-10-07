#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace utils::String {

	// return the pattern itself ( v.size() == 1 ) if there is nothing to split es. "hello" -> "hello", "  hello " -> "hello"
	std::vector<std::string> split(const std::string_view &s, const std::string_view &set);

}