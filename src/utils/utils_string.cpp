#include "utils_string.hpp"

#include <algorithm>

std::vector<std::string> split(const std::string_view &s, const std::string_view &set) noexcept {

	std::vector<std::string> vct(0);
	std::string_view::iterator it[2] {s.begin(), s.end()};

	const static auto &e_contained     = [&set](const char e) { return set.find(e) == std::string_view::npos; };
	const static auto &e_not_contained = [&set](const char e) { return set.find(e) != std::string_view::npos; };

	while (1) {

		if ((it[0] = std::find_if(it[0], s.end(), e_contained)) == s.end())
			return vct;

		if ((it[1] = std::find_if(it[0], s.end(), e_not_contained)) == s.end()) {
			vct.emplace_back(it[0], s.end());
			return vct;
		}

		vct.emplace_back(it[0], it[1]);
		it[0] = it[1];
	}

	return vct;
}
