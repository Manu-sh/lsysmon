#pragma once

#include <regex>

// including regex slow down compilation
namespace regex_utils {

	// literal constructor for regex
	static std::regex operator""_r(const char *regex, [[maybe_unused]] size_t dummy) {
		return std::regex{regex, std::regex::ECMAScript};
	}

	// literal constructor for case insensitive regex
	static std::regex operator""_ri(const char *regex, [[maybe_unused]] size_t dummy) {
		return std::regex{regex, std::regex::ECMAScript|std::regex::icase};
	} 

}