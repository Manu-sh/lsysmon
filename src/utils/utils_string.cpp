#include "utils_string.hpp"
#include "utils_line.hpp"

#include <algorithm>
#include <sstream>

std::vector<std::string> utils::String::split(const std::string_view &s, const std::string_view &set) {

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


std::string utils::String::map_file(const std::string &path) {

	using namespace utils::Line;

	std::stringstream ss;
	ifstream_l ifs;

	ifs.default_exceptions();
	ifs.open(path);

	std::copy(
		ifs.begin(), 
		ifs.end(),
		std::ostream_iterator<std::string>(ss, "\n") // don't use on windows obv
	);

	return ss.str();
}