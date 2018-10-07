#pragma once

#include <string>
#include <istream>
#include <fstream>
#include <iterator>
#include <functional>

namespace utils::Line {

	struct Line: std::string {};
	const static auto &npos = std::string::npos;

	/* getline from istream: Reaching the End-of-File sets the eofbit. But note that operations that reach 
	the End-of-File may also set the failbit if this makes them fail (thus setting both eofbit and failbit). */
	static std::istream & operator>>(std::istream &is, Line &l) {

		try {
			std::getline(is, l);
		} catch (std::ifstream::failure &e) {
			if (!is.eof()) throw;
		}

		return is;
	}

	// is can't be const they aren't const iterator
	static std::istream_iterator<Line> begin(std::istream &is) {
		return std::istream_iterator<Line>(is);
	}

	static const std::istream_iterator<Line> end([[maybe_unused]] const std::istream &dummy) {
		const static auto end = std::istream_iterator<Line>();
		return end;
	}

	class ifstream_l: public std::ifstream {

		public:
			using std::ifstream::ifstream;
			ifstream_l(const ifstream_l &) = delete;
			ifstream_l & operator=(const ifstream_l &) = delete;

			void each_line(std::function<void(const Line &)> f);
			void default_exceptions();
			std::istream_iterator<Line> begin();
			const std::istream_iterator<Line> end();

	};

}
